package plus.gmsure.server.controller;

import org.slf4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.ModelAndView;
import plus.gmsure.server.service.TestService;

import javax.annotation.Resource;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * @author GMsure
 * @since 2022/3/20 0020 0:08
 */
@RestController
@RequestMapping("/data/test")
@CrossOrigin(allowedHeaders = "*",value = "*",origins = "*",methods = {RequestMethod.GET,RequestMethod.POST},maxAge = 3600)
public class MainController {
    @Resource
    private TestService testService;
    @Resource
    private Logger logger;
    @Qualifier("TestWebSocketService")//bean过滤
    @Autowired
    private plus.gmsure.server.service.impl.WebSocketServer WebSocketServer;

    //再加一个正常接收和异常接收的计数

    private static int criT = 0;
    private static Integer [] tmpTmper = new Integer[7];
    private static int criM = 0;
    private static Integer [] tmpMoiser = new Integer[7];//之后可以用自定义队列一减一加做滚动摘取
    private static int criC = 0;
    private static int lastCO2 = 423;
    private static Integer [] tmpCO2cct = new Integer[7];

    public MainController(){
        for (int i = 0; i < 7; i++) {
            tmpTmper[i] = 0;
            tmpMoiser[i] = 0;
            tmpCO2cct[i] = 0;
        }
    }

    /**
     * 数据库查询测试
     * @return
     */
    @RequestMapping("/query")
    public String query(){
        logger.info(testService.queryAllTestPojo().toString());
        return testService.queryAllTestPojo().toString();
    }

    /**
     * 主请求
     * @param id
     * @param val
     * @return
     * @throws IOException
     */
    @PostMapping("/hello")
    public String hello(@RequestParam("id") Integer id,
                                    @RequestParam("val") String val) throws IOException {
        val = val.trim();
        logger.info("id：" + id + "\tval：" + val);
        String json = MandT2Json(val);
        logger.info("\n" + json);

        if(plus.gmsure.server.service.impl.WebSocketServer.getOnlineCount() > 0){

            plus.gmsure.server.service.impl.WebSocketServer.sendInfo(json,"1");
        }
        return "len：" + id + "\nval：" + val;
    }


    /**
     * 页面请求
     * @param userId
     * @return
     */
    @GetMapping("/index/{userId}")
    public ModelAndView socket(@PathVariable String userId) {
        ModelAndView mav = new ModelAndView("/socket1");
        mav.addObject("userId", userId);
        return mav;
    }

    /**
     *  推送数据接口
     * @param cid
     * @param message
     * @return
     */
    @RequestMapping("/socket/push/{cid}")
    public Map pushToWeb(@PathVariable String cid, String message) {
        Map<String,Object> result = new HashMap<>();
        try {
            WebSocketServer.sendInfo(message, cid);
            result.put("code", cid);
            result.put("msg", message);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return result;
    }

    /**
     * 温湿度十六进制转Json
     * 处理报文：len：73   val：01 03 04 02 1b 00 f8 8a 09   02 03 02 00 00 fc 44   03 03 02 01 9a 40 7f
     * @param HexVal 十六进制应答报文
     * @return
     */
    private String MandT2Json(String HexVal){
        String json ="";
        String[] vals = HexVal.split(" ");
        String Moisture = null;
        String Temperature = null;
        String SignBit = null;
        String NH3 = null;
        String CO2 = null;
        try {
            Moisture = vals[3] + vals[4];
            Temperature = vals[5] + vals[6];
            SignBit = "";
            NH3 = vals[14] + vals[15];
            CO2 = vals[23] + vals[24];
        } catch (Exception e) {
            logger.warn("Mistake message, data missing! ", e);
            logger.warn("Wait for the next piece of data to arrive. ");
//            e.printStackTrace();
        }
        Integer Moi = Integer.parseInt(Moisture,16);
        if (Moi >= 1000){
            Moi = avgArr(tmpMoiser);
        }else {
            tmpMoiser[criM++] = Moi;
            criM %= 7;
        }
        Integer Tmp = Integer.parseInt(Temperature,16);
        if(vals[5].equals("ff")) {
            Integer FFFF = Integer.parseInt("FFFF",16);
            Tmp = FFFF - Tmp + 1;
            SignBit = "-";
        }
        if (Tmp > 1000 || Tmp < -500){//一般会出现3000-5000的跳变 偶尔会出现-65000这样的跳变
            Tmp = avgArr(tmpTmper);
        }else {
            tmpTmper[criT++] = Tmp;
            criT %= 7;
        }
        Integer NH3cct = Integer.parseInt(NH3,16);
        NH3cct =  0;//氨气不测了
        Integer CO2cct = Integer.parseInt(CO2,16);
        if (CO2cct >= 2 * lastCO2 + 1 && CO2cct <= 3 * lastCO2){
            CO2cct =  ((CO2cct / 2) + avgArr(tmpCO2cct)) / 2; //二氧化碳跳变一般为二倍
        }else if (CO2cct > 1.57 * lastCO2 && CO2cct <= 2* lastCO2){
            CO2cct = ( (int)(CO2cct / 1.67) + avgArr(tmpCO2cct) * 2) / 3; //跳变一点五五倍时 按照均值权大 做3份均值
        } else if (CO2cct > 3 * lastCO2){
            CO2cct = avgArr(tmpCO2cct);
        } else {
            tmpCO2cct[criC++] = CO2cct;
            lastCO2 = CO2cct;
            criC %= 7;
        }

        //System.currentTimeMillis();
        json = "{ \"Moisture\": " + Moi + ", \"Temperature\": " + SignBit + Tmp +", \"NH3cct\": " + NH3cct + ", \"CO2cct\": " + CO2cct + ", \"TimeMillis\": " + System.currentTimeMillis() + " }";
        return json;
    }

    private Integer avgArr(Integer[] arr){
        int sum = 0, cnt = 0;
        for (int i = 0; i < arr.length; i++) {
            if (arr[i] != 0 && arr[i] != null) {
                sum += arr[i];
                cnt++;
            }
        }

        return cnt == 0 ? 0 : sum / cnt;
    }

}

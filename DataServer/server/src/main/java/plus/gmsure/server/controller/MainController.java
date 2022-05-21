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

    /**
     *
     * @return
     */
    @RequestMapping("/query")
    public String query(){
        logger.info(testService.queryAllTestPojo().toString());
        return testService.queryAllTestPojo().toString();
    }

    /**
     *
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
        logger.info(json);

        if(plus.gmsure.server.service.impl.WebSocketServer.getOnlineCount() > 0){

            plus.gmsure.server.service.impl.WebSocketServer.sendInfo(json,"1");
        }
        return "id：" + id + "\nval：" + val;
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
     * @param HexVal 十六进制应答报文
     * @return
     */
    private String MandT2Json(String HexVal){
        String json ="";
        String vals[] = HexVal.split(" ");
        String Moisture = vals[3] + vals[4];
        String Temperature = vals[5] + vals[6];
        Integer Moi = Integer.parseInt(Moisture,16);
        Integer Tmp = Integer.parseInt(Temperature,16);
        json = "{ \"Moisture\": " + Moi + ", \"Temperature\": " + Tmp +" }";
        return json;
    }


}

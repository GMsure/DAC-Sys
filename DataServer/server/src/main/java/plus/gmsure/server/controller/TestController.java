package plus.gmsure.server.controller;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import plus.gmsure.server.service.TestService;

import javax.annotation.Resource;

/**
 * @author GMsure
 * @since 2022/3/20 0020 0:08
 */
@RestController
@RequestMapping("/data/test")
@CrossOrigin(allowedHeaders = "*",value = "*",origins = "*",methods = {RequestMethod.GET,RequestMethod.POST},maxAge = 3600)
public class TestController {
    @Resource
    private TestService testService;
    @Resource
    private Logger logger;
    @RequestMapping("/query")
    public String query(){
        logger.info(testService.queryAllTestPojo().toString());
        return testService.queryAllTestPojo().toString();
    }
    @PostMapping("/hello")
    public String hello(@RequestParam("id") Integer id,
                                    @RequestParam("val") String val) {
        logger.info("id：" + id + "\tval：" + val);
        return "id：" + id + "\nval：" + val;
    }
}

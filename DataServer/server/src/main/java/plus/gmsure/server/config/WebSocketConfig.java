package plus.gmsure.server.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.server.standard.ServerEndpointExporter;

/**
 * @author GMsure
 * @since 2022/5/21 0021 10:35
 */
@Configuration
public class WebSocketConfig {
    //声明Bean 启用 Springboot 对 WebSocket 的支持
    @Bean
    public ServerEndpointExporter serverEndpointExporter() {
        return new ServerEndpointExporter();
    }

    //业务Service 需要在这里提前注入
//    @Autowired
//    private void setRedisService(IDetRecordService detRecordService){
//        TestWebSocketServer.detRecordService = detRecordService;
//    }
}
package plus.gmsure.server.controller;

import java.io.IOException;
import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.PathParam;
import javax.websocket.server.ServerEndpoint;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
/**
 * @author GMsure
 * @since 2022/5/21 0021 10:19
 */
@ServerEndpoint(value="/data/websocketTest/{userId}")
public class TestSocketController {
    private Logger logger = LoggerFactory.getLogger(TestSocketController.class);

    private static String userId;

    //连接时执行
    @OnOpen
    public void onOpen(@PathParam("userId") String userId,Session session) throws IOException{
        this.userId = userId;
        logger.debug("新连接：{}",userId);
    }

    //关闭时执行
    @OnClose
    public void onClose(){
        logger.debug("连接：{} 关闭",this.userId);
    }

    //收到消息时执行
    @OnMessage
    public void onMessage(String message, Session session) throws IOException {
        logger.debug("收到用户{}的消息{}",this.userId,message);
        session.getBasicRemote().sendText("收到 "+this.userId+" 的消息 "); //回复用户
    }

    //连接错误时执行
    @OnError
    public void onError(Session session, Throwable error){
        logger.debug("用户id为：{}的连接发送错误",this.userId);
        error.printStackTrace();
    }

}

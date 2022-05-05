package plus.gmsure.server;

import org.mybatis.spring.annotation.MapperScan;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.config.BeanFactoryPostProcessor;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Import;
import org.springframework.transaction.annotation.EnableTransactionManagement;
import plus.gmsure.server.config.DruidConfig;

@SpringBootApplication
@EnableTransactionManagement
@MapperScan("plus.gmsure.server.mapper")
@Import(DruidConfig.class)
public class DataServerApplication {

    public static void main(String[] args) {
        SpringApplication.run(DataServerApplication.class, args);
    }

    @Bean
    public static BeanFactoryPostProcessor removeTomcatWebServerCustomizer() {
        return (beanFactory) ->
                ((DefaultListableBeanFactory)beanFactory).removeBeanDefinition("tomcatWebServerFactoryCustomizer");
    }
    @Bean
    public Logger logger(){
        return LoggerFactory.getLogger(DataServerApplication.class);
    }
}

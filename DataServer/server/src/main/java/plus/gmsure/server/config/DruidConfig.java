package plus.gmsure.server.config;

import com.alibaba.druid.pool.DruidDataSource;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.sql.DataSource;

/**
 * @author GMsure
 * @since 2022/3/20 0020 1:42
 */
@Configuration
public class DruidConfig {
    @Bean
    @ConfigurationProperties(prefix = "druid")
    public DataSource dataSource(){
        return new DruidDataSource();
    }
}


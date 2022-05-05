package plus.gmsure.server.mapper;

import org.apache.ibatis.annotations.Mapper;
import plus.gmsure.dataserver.pojo.TestPojo;

/**
 * @author GMsure
 * @since 2022/3/20 0020 0:08
 */
@Mapper
public interface TestMapper {
    TestPojo queryAll();
}

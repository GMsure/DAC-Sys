package plus.gmsure.server.service.impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import plus.gmsure.dataserver.pojo.TestPojo;
import plus.gmsure.server.mapper.TestMapper;
import plus.gmsure.server.service.TestService;

/**
 * @author GMsure
 * @since 2022/3/20 0020 0:07
 */
@Service("TestService")
public class TestServiceImpl implements TestService {
    @Autowired
    TestMapper testMapper;
    @Override
    public TestPojo queryAllTestPojo() {
        return testMapper.queryAll();
    }
}

package plus.gmsure.dataserver.pojo;

/**
 * @author GMsure
 * @since 2022/3/20 0020 1:16
 */
public class TestPojo {
    private Integer testId;
    private String val;

    public Integer getTestId() {
        return testId;
    }

    public void setTestId(Integer testId) {
        this.testId = testId;
    }

    public String getVal() {
        return val;
    }

    public void setVal(String val) {
        this.val = val;
    }

    @Override
    public String toString() {
        return "TestPojo{" +
                "testId=" + testId +
                ", val='" + val + '\'' +
                '}';
    }
}

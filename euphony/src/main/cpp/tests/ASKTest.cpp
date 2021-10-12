#include <gtest/gtest.h>
#include <Definitions.h>
#include <ASK.h>
#include <FFTProcessor.h>
#include <tuple>
#include <ASCIICharset.h>
#include <HexVector.h>
#include <Packet.h>

using namespace Euphony;

typedef std::tuple<std::string, int> TestParamType;

class ASKTestFixture : public ::testing::TestWithParam<TestParamType>{

public:
    void createASK() {
        EXPECT_EQ(ask, nullptr);
        ask = new ASK();
        ASSERT_NE(ask, nullptr);
    }

    ASK* ask = nullptr;
    FFTProcessor* fft = nullptr;
};

TEST_P(ASKTestFixture, ASKModulationLengthTest){
    createASK();

    string source;
    string expectedModulateResult;
    int expectedCodeLength;

    std::tie(source, expectedCodeLength) = GetParam();
    HexVector hexVector = ASCIICharset().encode(source);
    auto *packet = new Packet(BaseType::BASE2, hexVector);
//    EXPECT_EQ(packet->toString(), expectedModulateResult);

    auto modulateASK = ask->modulate(packet->toString());
    EXPECT_EQ(modulateASK.size(), expectedCodeLength);

//    auto demodulateResult = ask->demodulate(modulateASK);
//    EXPECT_EQ(source, demodulateResult->getPayloadStr());
}

TEST_F(ASKTestFixture, ASKModulationStringTest){
    createASK();

    std::string source = "a";
    std::string expectedResult = "S0110000110010111";

    HexVector hv = ASCIICharset().encode(source);
    auto pkt = new Packet(BaseType::BASE2, hv);
    EXPECT_EQ(pkt->toString(), expectedResult);
}

INSTANTIATE_TEST_CASE_P(
        ASKTestSuite,
        ASKTestFixture,
        ::testing::Values(
                TestParamType("0", 17),
                TestParamType("1", 1),
                TestParamType("2", 1),
                TestParamType("3", 1),
                TestParamType("4", 1),
                TestParamType("5", 1),
                TestParamType("6", 1),
                TestParamType("7", 1),
                TestParamType("8", 1),
                TestParamType("9", 1),
                TestParamType("10", 2)
                )
        );
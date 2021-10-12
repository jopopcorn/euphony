#include "../ASK.h"
#include "../FFTProcessor.h"
#include "../Definitions.h"
#include "../WaveBuilder.h"
#include "../Base16Exception.h"
#include "../Packet.h"
#include <cmath>

using namespace Euphony;

ASK::ASK() : fftModel(std::make_unique<FFTProcessor>(kFFTSize, kSampleRate)) {}

WaveList ASK::modulate(Packet *packet) {
    return this->modulate(packet->toString());
}

WaveList ASK::modulate(std::string code) {
    vector<shared_ptr<Wave>> result;

    for (char c: code) {
        switch (c) {
            case '0':
                result.push_back(
                        Wave::create()
                                .vibratesAt(0)
                                .setSize(kBufferSize)
                                .setCrossfade(BOTH)
                                .build()
                );
                break;
            case '1':
                result.push_back(
                        Wave::create()
                                .vibratesAt(kStandardFrequency)
                                .setSize(kBufferSize)
                                .setCrossfade(BOTH)
                                .build()
                );
                break;
            case 'S':
                result.push_back(
                        Wave::create()
                                .vibratesAt(kStartSignalFrequency)
                                .setSize(kBufferSize)
                                .setCrossfade(BOTH)
                                .build()
                );
                break;
        }
    }

    return result;
}

std::shared_ptr<Packet> ASK::demodulate(const WaveList &waveList) {
    HexVector hexVector = HexVector(waveList.size());

    for (const auto &wave : waveList) {
        auto vectorInt2Source = wave->getInt16Source();
        int16_t *int16Source = &vectorInt2Source[0];
        float *resultBuf = fftModel->makeSpectrum(int16Source);
//        hexVector.pushBack();
    }
    return std::make_shared<Packet>(BaseType::BASE2, hexVector);
}

std::shared_ptr<Packet>
ASK::demodulate(const float *source, int sourceLength, int bufferSize) {
    int dataSize = sourceLength / bufferSize;
    HexVector hexVector = HexVector(dataSize);

    WaveList waveList;
    for(int i=0; i<dataSize; i++){
        waveList.push_back(std::make_shared<Wave>(source + (i * bufferSize), bufferSize));
    }
    return demodulate(waveList);
}

#pragma once
#include <AL/alut.h>

namespace ginkgo {
	struct AudioBuffer
	{
		ALuint sound; //sound buffer

		long lengthSamples; //number of samples in the sound

		int channels; //mono=1 stereo=2

		long samplingRate; //samples/sec (441000 or 22050 is standard)
	};
}
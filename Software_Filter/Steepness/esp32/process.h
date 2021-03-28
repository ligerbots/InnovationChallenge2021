#include <StandardCplusplus.h>
#include <vector>
#include <tuple>
using namespace std;

namespace process{
    const int sampling_frequency = 16000;
    const int num_samples = 512;
    const int iters = 100;

    const float freq_cutoff_low = 2000;
    const float freq_cutoff_high = 8000;

    const int low_index = freq_cutoff_low * num_samples / sampling_frequency;
    const int high_index = freq_cutoff_high * num_samples / sampling_frequency;

    vector<tuple<int, float>> inflections; // (Index, Height)

    double real_buf[num_samples];
    double imag_buf[num_samples];

    arduinoFFT FFT = arduinoFFT(real_buf, imag_buf, num_samples, sampling_frequency);

    unsigned long start;

    void process_fft(){
        // sampling_frequency/num_samples*i = freq

        FFT.DCRemoval();
        FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        FFT.Compute(FFT_FORWARD);
        FFT.ComplexToMagnitude();

        for(int i=0;i<num_samples;i++){ // What does this do? Is this to remove all imaginaries?
            imag_buf[i] = 0;
        }
        

        bool isAscending = true;
        float lowestFreq = real_buf[low_index]; 

        for(int i=low_index+1; i<=high_index; i++){    // Loop through range of interest
            float new_point = real_buf[i];
            float prev_point = real_buf[i-1];
            float difference = new_point - prev_point;

            if(isAscending == true && difference <= 0 ){
                isAscending = false;
                inflections.push_back(make_tuple(i, new_point));         // Store point when derivative changes
            }
            else if (isAscending == false && difference >= 0){
                inflections.push_back(make_tuple(i, new_point));         // Store point when derivative changes
            }
        }

        // for(int i; i<inflection_points.size )
    }

    void init(){
        pinMode(LET_BUILTIN, OUTPUT)
    }
}

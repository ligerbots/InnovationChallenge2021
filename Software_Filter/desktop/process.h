namespace process{
    const int sampling_frequency = 16000;
    const int num_samples = 512;
    const int iters = 100;

    const float freq_cutoff_low = 2000;
    const float freq_cutoff_high = 8000;

    const int low_index = freq_cutoff_low * num_samples / sampling_frequency;
    const int high_index = freq_cutoff_high * num_samples / sampling_frequency;

    const float min_height = 8e3f;
    const float max_margin = .3f;

    const int num_trackers = 5;

    int tracker_index[num_trackers];
    int tracker_count[num_trackers];
    const int max_peak_distance = 2;
    const int max_tracker_count = 5;
    const int tracker_cutoff = 3;
    const int num_peaks = 5;

    int peakIndexes[num_peaks];
    float peakHeights[num_peaks];

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

        for(int i=0;i<num_samples;i++){
            imag_buf[i] = 0;
        }

        bool isAscending = true;
        float localMinimum = 0;
        int localMinimumIndex = 0;
        float localMaximum = 0;
        int localMaximumIndex = 0;

        float previousLocalMinimum = real_buf[low_index];

        for(int i=0;i<num_peaks;i++){
            peakIndexes[i] = -1;
            peakHeights[i] = -1.f;
        }

        for(int i=low_index; i<=high_index; i++){
            if(isAscending){
                if(real_buf[i] * (1.f + max_margin) < localMaximum){
                    isAscending = false;
                    localMinimum = real_buf[i];
                    localMinimumIndex = i;
                }else{
                    if(real_buf[i] > localMaximum){
                        localMaximum = real_buf[i];
                        localMaximumIndex = i;
                    }
                }
            }else{
                if(real_buf[i] * (1.f - max_margin) > localMinimum){
                    isAscending = true;
                    float height = localMaximum - (previousLocalMinimum + localMinimum)/2;

                    int minIndex = 0;
                    for(int j=0;j<num_peaks;j++){
                        if(peakHeights[j]<peakHeights[minIndex]) minIndex = j;
                    }
                    if(height > peakHeights[minIndex] && height> min_height){
                        peakHeights[minIndex] = height;
                        peakIndexes[minIndex] = localMaximumIndex;
                    }

                    previousLocalMinimum = localMinimum;
                    localMaximum = real_buf[i];
                    localMaximumIndex = i;
                }else{
                    if(real_buf[i] < localMinimum){
                        localMinimum = real_buf[i];
                        localMinimumIndex = i;
                    }
                }
            }
        }
        /*
        for(int i=0;i<num_peaks;i++){
            if(peakIndexes[i]>=0){
                outfile << "L curr "<<peakIndexes[i] << " " << peakHeights[i] << "\n";
            }
        }*/

        for(int i=0;i<num_trackers;i++){
            if(tracker_count[i]>0){
                int closestPeakIndex = -1;
                int closestPeakDistance = max_peak_distance;

                int prevTrackerIndex = tracker_index[i];
                for(int j=0;j<num_peaks;j++){
                    if(peakIndexes[j]>=0){
                        int distance = abs(peakIndexes[j]-prevTrackerIndex);
                        if(distance <= closestPeakDistance){
                            closestPeakIndex = j;
                            closestPeakDistance = distance;
                        }
                    }
                }
                if(closestPeakIndex>=0){
                    if(tracker_count[i]<max_tracker_count){
                        tracker_count[i]++;
                    }
                    tracker_index[i] = peakIndexes[closestPeakIndex];

                    peakIndexes[closestPeakIndex] = -1;
                }else{
                    tracker_count[i]--;
                }
            }

        }
        /*
        std::cout<<"A: ";
        for(int i=0;i<num_trackers;i++){
            std::cout<<tracker_index[i]<<":"<<tracker_count[i]<<" ";
        }
        std::cout<<"\n";
        */
        for(int i=0;i<num_trackers;i++){
            if(tracker_count[i]<=0){
                int maxPeakIndex = -1;
                float maxPeakHeight = -1;
                for(int j=0;j<num_peaks;j++){
                    if(peakIndexes[j]>=0 && peakHeights[j] > maxPeakHeight){
                        maxPeakIndex = j;
                        maxPeakHeight = peakHeights[j];
                    }
                }
                if(maxPeakIndex>=0){
                    tracker_count[i] = 1;
                    tracker_index[i] = peakIndexes[maxPeakIndex];
                    peakIndexes[maxPeakIndex] = -1;

                }
            }
        }


        bool anyWhistle = false;

        for(int i=0;i<num_trackers;i++){
            if(tracker_count[i]>=tracker_cutoff){
                anyWhistle=true;
            }
        }

        #ifdef __DESKTOP__
            for(int i=0;i<num_trackers;i++){
                if(tracker_count[i]>=1){
                    outfile << "L curr "<<tracker_index[i] << " " << tracker_count[i] << "\n";
                }
            }

            for(int i=0; i<num_samples/2; i++){
                outfile << real_buf[i] << " ";
            }

            if(anyWhistle){
                outfile << "y";
            }else{
                outfile << "n";
            }

            outfile <<"\n";
        #endif

        #ifdef __ESP32__
        /*
            for(int i=0;i<num_trackers;i++){
                if(tracker_count[i]>=1){
                    Serial.print("L curr ");
                    Serial.print(tracker_index[i]);
                    Serial.print(" ");
                    Serial.print(tracker_count[i]);
                    Serial.println();
                }
            }
            for(int i=0; i<num_samples/2; i++){
                Serial.print(real_buf[i]);
                Serial.print(" ");
            }

            if(anyWhistle){
                Serial.print("y");
            }else{
                Serial.print("n");
            }

            Serial.println();
            */
        #endif


        #ifdef __ESP32__
            digitalWrite(LED_BUILTIN, anyWhistle);
        #endif


    }

    void init(){
        #ifdef __DESKTOP__
            std::cout<<"low "<<low_index<<" high "<<high_index<<"\n";
        #endif
        #ifdef __ESP32__
            pinMode(LED_BUILTIN, OUTPUT);
        #endif

    }
}
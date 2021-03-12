
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#define SAMPLE_RATE 16000
#define BASE_FREQ 50 //must be factor of sample rate
#define NUM_SAMPLES 512

#define BASE_NUM_SAMPLES (SAMPLE_RATE/BASE_FREQ)
#define TRIG_MULT 100 //must be less than INT32_MAX_VALUE / NUM_SAMPLES / INT16_MAX_VALUE

#define NUM_BINS (BASE_NUM_SAMPLES/2) // should be less than BASE_NUM_SAMPLES/2

int32_t base_freq_real[BASE_NUM_SAMPLES];
int32_t base_freq_imag[BASE_NUM_SAMPLES];

int32_t totals_real[NUM_BINS];
int32_t totals_imag[NUM_BINS];

int16_t prev_sound[NUM_SAMPLES];

void reset(){
    for(int i=0;i<BASE_NUM_SAMPLES;i++){
        base_freq_real[i] = (int)(cos(2.*M_PI*i/BASE_NUM_SAMPLES)*TRIG_MULT);
        base_freq_imag[i] = (int)(sin(2.*M_PI*i/BASE_NUM_SAMPLES)*TRIG_MULT);
    }
    for(int i=0;i<NUM_BINS;i++){
        totals_real[i]=0;
        totals_imag[i]=0;
    }
    for(int i=0;i<NUM_SAMPLES;i++){
        prev_sound[i]=0;
    }
}

int sound_index = 0;
int mod(int a, int b){
    return(((a%b)+b)%b);
}

void update(int16_t sound){
    int16_t old_sound = prev_sound[mod(sound_index,NUM_SAMPLES)];
    prev_sound[mod(sound_index,NUM_SAMPLES)]=sound;
    for(int i=0;i<NUM_BINS;i++){
        int old_index = mod((sound_index-NUM_SAMPLES)*i,BASE_NUM_SAMPLES);
        totals_real[i] -= base_freq_real[old_index]*old_sound;
        totals_imag[i] -= base_freq_imag[old_index]*old_sound;
    }
    for(int i=0;i<NUM_BINS;i++){
        int new_index = mod(i*sound_index,BASE_NUM_SAMPLES);
        totals_real[i] += base_freq_real[new_index]*sound;
        totals_imag[i] += base_freq_imag[new_index]*sound;
    }
    sound_index++;
}

std::vector<int16_t> read_sound_csv(const char* file_name){
    std::ifstream file(file_name);
    std::vector<int16_t> pts;

    while(true){
        int16_t x;
        file>>x;
        if(file.eof()) break;
        pts.push_back(x);
    }

    return(pts);
}

void print_mags(std::ofstream &file){

    for(int i=0;i< NUM_BINS;i++){
        float real = totals_real[i];
        float imag = totals_imag[i];
        file<<sqrt(real*real+imag*imag)<<" ";

    }
    file<<"\n";
}

int main() {

    std::ofstream outfile;
    outfile.open("spectrogram.txt");
    std::cout<<"???\n";

    reset();
    std::cout<<"reading\n";

    std::vector<int16_t> data = read_sound_csv("data.txt");
    std::cout<<"generating spectrogram\n";

    for(int i=0;i<data.size();i++){
        update(data[i]);
        print_mags(outfile);

        if(i%SAMPLE_RATE==0)std::cout<<"second "<<i/SAMPLE_RATE<<" / "<< (data.size()/SAMPLE_RATE) <<"\n";
    }
    outfile.close();
}

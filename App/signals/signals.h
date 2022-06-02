#ifndef _SIGNALS_H_
#define _SIGNALS_H_

class Signal {
public:
  template<typename T>
  T& SetFreq(freq) {
    freq_ = freq;
    return *this;
  }
  
  template<typename T>
  T& SetAmp(amp) {
    amp_ = amp;
    return *this;
  }
    
private:
  float freq_ = 1;
  float amp_ = 1;
};

#endif // #ifndef _SIGNALS_H_
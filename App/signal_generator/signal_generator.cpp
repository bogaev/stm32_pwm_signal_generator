#include "App/signal_generator/signal_generator.hpp"

SignalGenerator::SignalGenerator() :
    carrier_(Signal{}.Create(SIGNAL_TYPE_SINUS)),
    fmod_(Signal{}.Create(SIGNAL_TYPE_NONE)),
    amod_(Signal{}.Create(SIGNAL_TYPE_NONE))
    {}

SignalGenerator::SignalGenerator(tdSignalTypes_ carrier,
                         tdSignalTypes_ fmod,
                         tdSignalTypes_ amod)
  :
    carrier_(Signal{}.Create(carrier)),
    fmod_(Signal{}.Create(fmod)),
    amod_(Signal{}.Create(amod))
    {}

SignalGenerator::SignalGenerator(std::shared_ptr<Signal> carrier, 
                           std::shared_ptr<Signal> fmod,
                           std::shared_ptr<Signal> amod)
    : carrier_(carrier),
      fmod_(fmod),
      amod_(amod)
    {}

float SignalGenerator::operator()() {
  if(fmod_ && amod_) {
    GenerateFM().AddAM();
  }
  else if(fmod_) {
    GenerateFM();
  }
  else if(amod_) {
    GenerateCarrier().AddAM();
  }
  else {
    GenerateCarrier();
  }
  ++time_;
  return value_;
}

SignalGenerator& SignalGenerator::GenerateCarrier() {
  if(carrier_) {
    value_ = (*carrier_)(time_);
  }
  return *this;
}  

SignalGenerator& SignalGenerator::AddAM() {
  if(amod_) {
    value_ *= (*amod_)(time_);
  }
  return *this;
}  

SignalGenerator& SignalGenerator::GenerateFM() {
  if(fmod_) {
    value_ = (*carrier_).FM(time_, *fmod_);
  }
  return *this;
}

float SignalGenerator::GetAmp() {
  if(carrier_) {
    return (*carrier_).GetAmp();
  }
  return 0;
}

float SignalGenerator::GetFreq() {
  if(carrier_) {
    return (*carrier_).GetFreq();
  }
  return 0;
}

void SignalGenerator::SetSignal(uint8_t signal, uint8_t param, uint16_t value) {
  Reset();
  if(signal == UART_SIGNAL_CARRIER) {
    if(param == UART_PARAM_SIGNAL_TYPE) {
      carrier_ = Signal{}.Create(static_cast<tdSignalTypes_>(value));
      return;
    }
    (*carrier_).SetParam(param, value);
  }
  else if(signal == UART_SIGNAL_AMP_MOD) {
    if(param == UART_PARAM_SIGNAL_TYPE) {
      amod_ = Signal{}.Create(static_cast<tdSignalTypes_>(value));
      return;
    }
    (*amod_).SetParam(param, value);
  }
  else if(signal == UART_SIGNAL_FREQ_MOD) {
    if(param == UART_PARAM_SIGNAL_TYPE) {
      fmod_ = Signal{}.Create(static_cast<tdSignalTypes_>(value));
      return;
    }
    (*fmod_).SetParam(param,value);
  }
}

void SignalGenerator::Reset() {
  time_ = 0;
  value_ = 0;
}

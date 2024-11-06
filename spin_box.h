#pragma once
#include "global.h"

template <typename T>
class SpinBox;

template<typename T>
class ISpinnable {
public:
  virtual ~ISpinnable() = default;
  virtual void onSpin(const SpinBox<T>& sender, T value);
};

template<typename T>
class SpinBox {
public:
	SpinBox<T>(T min_val, T max_val, int tag = 0) :
		min_val_(min_val), max_val_(max_val), tag_(tag) {
		value_ = min_val_;
	}

  void setSpinListener(ISpinnable<T>* listener) {
    listener_ = listener;
  }

	inline T value() const {
		return value_;
	}

  inline int tag() const {
    return tag_;
  }

	void setValue(T value) {
		if (value == value_)
			return;
    if (value > max_val_) value_ = max_val_;
      else
    if (value < min_val_) value_ = min_val_;
      else
    value_ = value;
	}

	void increment() {
		if (++value_ > max_val_) {
			value_ = min_val_;
		}
    if (listener_) listener_->onSpin(*this, value_);
	}

	void decrement() {
    //On unsigned types reaching 0 causes underflow
		if (--value_ < min_val_) {
			value_ = max_val_;
		};
    if (listener_) listener_->onSpin(*this, value_);
	}

  void setMin(T min_val) {
    if (min_val < max_val_) {
      min_val_ = min_val;
    }
    if (value_ < min_val_) {
      value_ = min_val_;
      if (listener_) listener_->onSpin(*this, value_);
    }
  }

  void setMax(T max_val) {
    if (max_val > min_val_) {
      max_val_ = max_val;
    }
    if (value_ > max_val_) {
      value_ = max_val_;
      if (listener_) listener_->onSpin(*this, value_);
    }
  }

private:
	T min_val_;
	T max_val_;
  const int tag_;
	T value_;
  ISpinnable<T>* listener_ = {};
};
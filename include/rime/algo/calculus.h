//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2012-01-17 GONG Chen <chen.sst@gmail.com>
//

#ifndef RIME_CALCULUS_H_
#define RIME_CALCULUS_H_

#include <map>
#include <string>
#include <vector>
#include <stdint.h>
#include <boost/regex.hpp>
#include "spelling.h"

namespace rime {

class Calculation {
 public:
  using Factory = Calculation* (const std::vector<std::string>& args);

  Calculation() = default;
  virtual ~Calculation() = default;
  virtual bool Apply(Spelling* spelling) = 0;
  virtual bool addition() { return true; }
  virtual bool deletion() { return true; }
};

class Calculus {
 public:
  Calculus();
  void Register(const std::string& token, Calculation::Factory* factory);
  Calculation* Parse(const std::string& defintion);

 private:
  std::map<std::string, Calculation::Factory*> factories_;
};

// xlit/zyx/abc/
class Transliteration : public Calculation {
 public:
  static Factory Parse;
  bool Apply(Spelling* spelling);

 protected:
  std::map<uint32_t, uint32_t> char_map_;
};

// xform/x/y/
class Transformation : public Calculation {
 public:
  static Factory Parse;
  bool Apply(Spelling* spelling);

 protected:
  boost::regex pattern_;
  std::string replacement_;
};

// erase/x/
class Erasion : public Calculation {
 public:
  static Factory Parse;
  bool Apply(Spelling* spelling);
  bool addition() { return false; }

 protected:
  boost::regex pattern_;
};

// derive/x/X/
class Derivation : public Transformation {
 public:
  static Factory Parse;
  bool deletion() { return false; }
};

// fuzz/zyx/zx/
class Fuzzing : public Derivation {
 public:
  static Factory Parse;
  bool Apply(Spelling* spelling);
};

// abbrev/zyx/z/
class Abbreviation : public Derivation {
 public:
  static Factory Parse;
  bool Apply(Spelling* spelling);
};

}  // namespace rime

#endif  // RIME_CALCULUS_H_

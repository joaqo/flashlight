/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/**
 * Generic utilities which should not depend on ArrayFire / flashlight.
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "flashlight/libraries/common/String.h"
#include "flashlight/libraries/text/dictionary/Dictionary.h"
#include "flashlight/libraries/text/dictionary/Utils.h"
#include "flashlight/tasks/speech_recognition/common/Defines.h"

using fl::lib::text::Dictionary;

namespace fl {
namespace tasks {
namespace asr {

/* A series of vector to vector mapping operations */

std::vector<std::string> tknIdx2Ltr(const std::vector<int>&, const Dictionary&);

std::vector<std::string> tkn2Wrd(const std::vector<std::string>&);

std::vector<std::string> wrdIdx2Wrd(const std::vector<int>&, const Dictionary&);

std::vector<std::string> tknTarget2Ltr(std::vector<int>, const Dictionary&);

std::vector<std::string> tknPrediction2Ltr(std::vector<int>, const Dictionary&);

std::vector<int> tkn2Idx(
    const std::vector<std::string>& spelling,
    const Dictionary& tokenDict,
    int maxReps);

std::vector<int> validateIdx(std::vector<int> input, int unkIdx);

template <class T>
void remapLabels(std::vector<T>& labels, const Dictionary& dict) {
  if (FLAGS_eostoken) {
    int eosidx = dict.getIndex(kEosToken);
    while (!labels.empty() && labels.back() == eosidx) {
      labels.pop_back();
    }
  }
  if (FLAGS_replabel > 0) {
    labels = unpackReplabels(labels, dict, FLAGS_replabel);
  }
  auto trimLabels = [&labels](int idx) {
    if (!labels.empty() && labels.back() == idx) {
      labels.pop_back();
    }
    if (!labels.empty() && labels.front() == idx) {
      labels.erase(labels.begin());
    }
  };
  if (dict.contains(kSilToken)) {
    trimLabels(dict.getIndex(kSilToken));
  }
  if (!FLAGS_surround.empty()) {
    trimLabels(dict.getIndex(FLAGS_surround));
  }
};
} // namespace asr
} // namespace tasks
} // namespace fl
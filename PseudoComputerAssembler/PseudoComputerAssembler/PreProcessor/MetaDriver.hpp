#pragma once

#include <string>

namespace PreProcess {
  void StripComments(std::string &fileStream) ;
  void HandleDefinitions(std::string &fileStream) ;
  void CaseDesensitize(std::string &fileStream) ;
  void StripEmptyLines(std::string &fileStream) ;
}
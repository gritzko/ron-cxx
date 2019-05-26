#include "hash.hpp"

namespace ron {

const SHA2 SHA2::ZERO = SHA2::ParseHex("00000000000000000000000000000000");
const Uuid SHA2::FORM_ID{"sha2"};

};  // namespace ron

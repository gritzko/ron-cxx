#include "key.hpp"

namespace ron {

const Key Key::ZERO{Uuid::NIL, ZERO_RAW_FORM};
const Key Key::END{Uuid{Word::MAX_VALUE, Word::MAX_VALUE}, ERROR_NO_FORM};

bool Key::trace_by_key{false};

}  // namespace ron

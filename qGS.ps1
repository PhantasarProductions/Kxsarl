param(
    [string]$src   = "*nothing*",
    [string]$game  = "*nothing*"
)

if ($game -eq "game") {

echo '
#pragma once
#include <string>

namespace Kxsarl {
	namespace Game {
	}
}
' > Kxsarl/Headers/$src.hpp


echo '
#include <TQSG.hpp>
#include <TQSE.hpp>
#include <TQSA.hpp>

#include "AllHeaders.hpp"


using namespace std;
using namespace Slyvina;
using namespace Units;
using namespace JCR6;
using namespace TQSG;
using namespace TQSE;
using namespace TQSA;

namespace Kxsarl {  
	namespace Game {
	} 
} ' > Kxsarl/Source/$src.cpp

} else {

cho "
#pragma once
#include <string>

namespace Kxsarl {
}
" > Kxsarl/Headers/$src.hpp


echo '
#include <TQSG.hpp>
#include <TQSE.hpp>
#include <TQSA.hpp>

using namespace std;
using namespace Slyvina;
using namespace Units;
using namespace JCR6;
using namespace TQSG;
using namespace TQSE;
using namespace TQSA;

#include "AllHeaders.hpp"

namespace Kxsarl {  
} ' > Kxsarl/Source/$src.cpp

}


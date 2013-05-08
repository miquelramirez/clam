
__version__ = "1.0"

from Dummy_Engine import Dummy_Engine
try : from Clam_Engine import Clam_Engine
except ImportError: pass
try : from Jack_Engine import Jack_Engine
except ImportError: pass

from Network import Network



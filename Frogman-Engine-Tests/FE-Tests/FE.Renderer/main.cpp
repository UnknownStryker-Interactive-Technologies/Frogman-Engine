#include <FE/framework/framework.hpp>




// https://paroj.github.io/gltut/
class renderer : public FE::framework::application_base
{
	virtual bool set_up(_MAYBE_UNUSED_ int argc_p, _MAYBE_UNUSED_ char** argv_p) override
	{
		return true;
	}

	virtual int run(_MAYBE_UNUSED_ int argc_p, _MAYBE_UNUSED_ char** argv_p) override
	{
		return 0;
	}

	virtual void clean_up() override
	{
	}
};




// App configuration script
CONFIGURE_A_FROGMAN_ENGINE_PROJECT()

return new renderer;

END_OF_THE_FROGMAN_ENGINE_PROJECT_CONFIGURATION()
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <ClanLib/application.h>

class CL_Clan_Compiler : public CL_ClanApplication
{
	char *get_title()
	{
		return "Datafile Compiler";
	}
  
	int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			if (argc < 2)
			{
				std::cout << "Sample ClanLib Datafile Compiler - supports only core resources." << std::endl;
				std::cout << "(TGA, PCX, WAV, Integer, String, Boolean, Font)" << std::endl << std::endl;
				std::cout << "Usage:" << std::endl;
				std::cout << "clan_compiler inputfile.scr outputfile.dat" << std::endl;
			}
			else
			{
				/*
				 * we pass "true" to the CL_Setup???::init methods, since we only
				 * need to register the different resource types. This will allow
				 * clan_compiler to be used in a batch, when no graphics mode is
				 * available.
				 */
				CL_SetupCore::init(true);
				CL_SetupSound::init(true);
				CL_SetupDisplay::init(true);

				// If you add more CL_Setup??? here (JPEG, PNG, etc) you add
				// support for more resources.

				CL_DatafileCompiler::write(argv[1], argv[2]);

				CL_SetupDisplay::deinit();
				CL_SetupSound::deinit();
				CL_SetupCore::deinit();
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
		}

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} compiler;

#include "Client"

#ifdef _WINDOWS
int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow )
#else
int main( int argc, char** argv )
#endif
{
  fa::Client client;

  client.init();
  client.run();
  return 0;
}



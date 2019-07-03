#include <iostream>
#include <string>
#include <algorithm>

#include "state.h"
#include "buttonnamed.h"
#include "startpage.h"
#include "createmappage.h"
#include "gamepage.h"
#include "startpageinitdata.h"
#include "createmappageinitdata.h"
#include "gamepageinitdata.h"

#undef main        // Prevents SDL from overriding main()

int main()
{
    State MainState( "media/maps/high_score.txt" );

    {
        MainWindow window( "BATTLE CITY", "media/images/win_icon.png" );

        StartPageInitData     startPageInitData    ( window.getRenderer() );
        CreateMapPageInitData createMapPageInitData( window.getRenderer() );
        GamePageInitData      gamePageInitData     ( window.getRenderer() );

        window.attachPage( std::make_shared <StartPage>     ( startPageInitData, startPageInitData, &MainState, window.getRenderer() ));
        window.attachPage( std::make_shared <CreateMapPage> ( createMapPageInitData, &MainState, window.getRenderer() ));
        window.attachPage( std::make_shared <GamePage>      ( gamePageInitData, gamePageInitData, &MainState, window.getRenderer() ));
        MainState.attachWindow( window );
    }

    MainState.mainAction();

    return 0;
}

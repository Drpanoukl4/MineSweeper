
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct vec2D 
{

    int x;
    int y;

};

struct sNode_Cell
{

    vec2D pos;
    bool bisMine;
    bool bisHide;
    bool bisFlag = 0;
    int Number_Friends = 0;
    std::vector <sNode_Cell *> cell_Neighbours; // Neighbours

};

vec2D world_Size = { 15, 22 };
vec2D cell_Size = { 17, 17 };

sNode_Cell * sWorld = new sNode_Cell [ world_Size.x * world_Size.y ];

olc::Sprite * spr;

int Number_Mines = 0;

bool bGO = false;

bool Win = false;

bool xs = false;

std::vector< sNode_Cell *> nods;


class Shadow : public olc::PixelGameEngine
{

public:
	Shadow()
	{
		sAppName = "Example";
	}

public:

    void GameOver () 
    {

        bGO = true; 

    }

    void Reset() 
    {

        Number_Mines = 0;

        Win = false;

        for( int x = 0; x < world_Size.x; x ++ ) 
        {

            for( int y = 0; y < world_Size.y; y ++ ) 
            {

                sWorld[ y * world_Size.x + x ].pos.x = x;
                sWorld[ y * world_Size.x + x ].pos.y = y;
                sWorld[ y * world_Size.x + x ].bisMine = (rand()% 40) < 9;
                sWorld[ y * world_Size.x + x ].bisHide = 1;
                sWorld[ y * world_Size.x + x ].bisFlag = 0;
                sWorld[ y * world_Size.x + x ].Number_Friends = 0;
                sWorld[ y * world_Size.x + x ].cell_Neighbours.clear(); // Neighbours


            }

        }

        for( int x = 0; x < world_Size.x; x ++ ) 
        {

            for( int y = 0; y < world_Size.y; y ++ ) 
            {

                if( y > 0 )
					sWorld[ y * world_Size.x + x ].cell_Neighbours.push_back( &sWorld[ (y - 1) * world_Size.x + ( x + 0) ] );
				if( y < world_Size.y - 1 )
					sWorld[ y * world_Size.x + x ].cell_Neighbours.push_back( &sWorld[ (y + 1) * world_Size.x + ( x + 0) ] );
				if( x > 0 )
					sWorld[ y * world_Size.x + x ].cell_Neighbours.push_back( &sWorld[ (y + 0) * world_Size.x + ( x - 1) ] );
				if( x < world_Size.x - 1 )
					sWorld[ y * world_Size.x + x ].cell_Neighbours.push_back( &sWorld[ (y + 0) * world_Size.x + ( x + 1) ] );
                if( y > 0 && x > 0 )
                    sWorld[ y * world_Size.x + x ].cell_Neighbours.push_back( &sWorld[ (y - 1) * world_Size.x + ( x - 1) ] );
                if( y < world_Size.y - 1 && x > 0 )
                    sWorld[ y * world_Size.x + x ].cell_Neighbours.push_back( &sWorld[ (y + 1) * world_Size.x + ( x - 1) ] );
                if( y > 0 && x < world_Size.x - 1 )
                    sWorld[ y * world_Size.x + x ].cell_Neighbours.push_back( &sWorld[ (y - 1) * world_Size.x + ( x + 1) ] );
                if( y < world_Size.y - 1 && x < world_Size.x - 1 )
                    sWorld[ y * world_Size.x + x ].cell_Neighbours.push_back( &sWorld[ (y + 1) * world_Size.x + ( x + 1) ] );

            }

        }

        for( int x = 0; x < world_Size.x; x ++ ) 
        {

            for( int y = 0; y < world_Size.y; y ++ ) 
            {

                if( sWorld[ y * world_Size.x + x ].bisMine  ) 
                {

                    Number_Mines ++;

                }else 
                {

                    for( auto &n : sWorld[ y * world_Size.x + x ].cell_Neighbours) 
                    {

                        if( n->bisMine == 1 ) 
                        {

                            sWorld[ y * world_Size.x + x ].Number_Friends ++;

                        }

                    }

                }

            }

        }

        bGO = false;

    }

	bool OnUserCreate() override
	{

        srand( time(0) );

        spr = new olc::Sprite("Mine.png");

        Reset ();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        
        Clear(olc::DARK_GREY);

        vec2D vCursor = { GetMouseX() / cell_Size.x, GetMouseY() / cell_Size.y };

        if( GetMouse(0).bPressed ) 
        {   
            if(bGO == 0){

                if(sWorld[ vCursor.y * world_Size.x + vCursor.x ].bisMine == 1 && sWorld[ vCursor.y * world_Size.x + vCursor.x ].bisFlag == 0) 
                {

                    sWorld[ vCursor.y * world_Size.x + vCursor.x ].Number_Friends = 9;

                    GameOver();

                }else if(sWorld[ vCursor.y * world_Size.x + vCursor.x ].bisFlag == 0 && sWorld[ vCursor.y * world_Size.x + vCursor.x ].bisHide == 1) 
                {

                    sWorld[ vCursor.y * world_Size.x + vCursor.x ].bisHide = 0;

                    if( sWorld[ vCursor.y * world_Size.x + vCursor.x ].Number_Friends < 1 )
                    {

                        Clean(sWorld[ vCursor.y * world_Size.x + vCursor.x ]);

                        for( auto &n : sWorld[ vCursor.y * world_Size.x + vCursor.x ].cell_Neighbours ) 
                        {

                            Clean(*n);
                            for( auto &i : n->cell_Neighbours ) 
                            {

                                Clean(*i);
                                for( auto &o : i->cell_Neighbours ) 
                                {

                                    Clean(*o);

                                }

                            }

                        }

                    }

                }
            }
        }

        if( GetMouse(1).bPressed ) 
        {
            if(bGO == 0){
                
                
                if(sWorld[ vCursor.y * world_Size.x + vCursor.x ].bisHide) 
                {

                    sWorld[ vCursor.y * world_Size.x + vCursor.x ].bisFlag = !sWorld[ vCursor.y * world_Size.x + vCursor.x ].bisFlag;

                    if(sWorld[ vCursor.y * world_Size.x + vCursor.x ].bisFlag == 1) 
                    {

                        Number_Mines --;

                    }else 
                    {

                        Number_Mines ++;

                    }

                }
            
            }

        }

        for( int x = 0; x < world_Size.x; x ++ ) 
        {

            for( int y = 0; y < world_Size.y; y ++ ) 
            {

                switch( sWorld[ y * world_Size.x + x ].bisHide ) 
                {

                    case 0:

                        switch( sWorld[ y * world_Size.x + x ].Number_Friends ) 
                        {

                            case 0:

                                DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 1 * cell_Size.x, 0 * cell_Size.y, cell_Size.x, cell_Size.y);
                                break;

                            case 1:

                                DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 1 * cell_Size.x, 1 * cell_Size.y, cell_Size.x, cell_Size.y);
                                break;
                            case 2:

                                DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 2 * cell_Size.x, 1 * cell_Size.y, cell_Size.x, cell_Size.y);
                                break;
                            case 3:

                                DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 3 * cell_Size.x, 1 * cell_Size.y, cell_Size.x, cell_Size.y);
                                break;
                            case 4:

                                DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 0 * cell_Size.x, 2 * cell_Size.y, cell_Size.x, cell_Size.y);
                                break;
                            case 5:

                                DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 1 * cell_Size.x, 2 * cell_Size.y, cell_Size.x, cell_Size.y);
                                break;
                            case 6:

                                DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 2 * cell_Size.x, 2 * cell_Size.y, cell_Size.x, cell_Size.y);
                                break;
                            case 7:

                                DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 3 * cell_Size.x, 2 * cell_Size.y, cell_Size.x, cell_Size.y);
                                break;
                            case 8:

                                DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 0 * cell_Size.x, 3 * cell_Size.y, cell_Size.x, cell_Size.y);
                                break;
                            
                        }

                        break;

                    case 1:

                        DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 0 * cell_Size.x, 0 * cell_Size.y, cell_Size.x, cell_Size.y);

                        break;

                }

                if( sWorld[ y * world_Size.x + x ].bisFlag ) 
                {

                    DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 0 * cell_Size.x, 1 * cell_Size.y, cell_Size.x, cell_Size.y);

                }

            }

        }

        DrawString( 10, 385, std::to_string(Number_Mines ));

        if( Number_Mines <= 0 ) 
        {

            Number_Mines = 0;

            Win = true;

        }

        if(Win) 
        {

            DrawString( 180, 385, "Win", olc::BLUE);

        }

        if(bGO || xs == 1 ) 
        {

            DrawString( 180, 385, "Game Over", olc::RED);

            for( int x = 0; x < world_Size.x; x ++ ) 
            {

                for( int y = 0; y < world_Size.y; y ++ ) 
                {
                    if( sWorld[ y * world_Size.x + x ].bisMine )
                        DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 2 * cell_Size.x, 0 * cell_Size.y, cell_Size.x, cell_Size.y);
                    if(sWorld[ y * world_Size.x + x ].Number_Friends == 9)
                        DrawPartialSprite( x * cell_Size.x, y * cell_Size.y, spr, 3 * cell_Size.x, 0 * cell_Size.y, cell_Size.x, cell_Size.y);
                }

            }
        }

        if(GetKey(olc::Key::ENTER).bPressed) 
        {

            Reset();

        }

        if(GetKey(olc::Key::O).bPressed) 
        {

            xs = !xs;

        }

        if( bGO ) 
        {

            DrawPartialSprite( ScreenWidth() / 2 - 8, 375, spr, 2 * cell_Size.x, 3 * cell_Size.y, cell_Size.x, cell_Size.y);
        
        }else if( Win )
        {

            DrawPartialSprite( ScreenWidth() / 2 - 8, 375, spr, 3 * cell_Size.x, 3 * cell_Size.y, cell_Size.x, cell_Size.y);

        }else 
        {

            DrawPartialSprite( ScreenWidth() / 2 - 8, 375, spr, 1 * cell_Size.x, 3 * cell_Size.y, cell_Size.x, cell_Size.y);

        }

		return true;
	}

    void Clean( sNode_Cell nod ) 
    {

        nods.clear();

        for( auto &n : nod.cell_Neighbours) 
        {

            if( n->bisMine == 1 || n-> bisFlag == 1 ) 
            {

                break; 

            }else 
            {


                nods.push_back( n );

            }

        }

        if( nods.size() >= 4)
        {

            for( auto &n : nods ) 
            {

                n->bisHide = 0;

            }

        }

    }
};

int main()
{
	Shadow demo;
	if (demo.Construct(254, 397, 2, 2))
		demo.Start();

	return 0;
}
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<sstream>
using namespace sf;

//function declaration
void updateBranches(int seed);
const int NUM_BRANCHES=6;
Sprite branches[NUM_BRANCHES];

//where the branch is drawn?
enum class side{LEFT,RIGHT,NONE};
side branchPositions[NUM_BRANCHES];

int main(){
    //create videomode
    VideoMode vm(1366,768);
    //create window
    RenderWindow window(vm,"Timber");

    //create texture for background
    Texture backgroundTexture;//tiffin
    backgroundTexture.loadFromFile("graphics/background.png");//load tiffin ka andr k maal
    //for linking the loaded file- to hold it.
    Sprite backgroundSprite;//tiffin ka dhakan- hold and fix
    backgroundSprite.setTexture(backgroundTexture);//texture sprite me ja chuka h,ab sprite use hoga texture access krne ke liye.
    backgroundSprite.setPosition(-150,-200);

    Texture treeTexture;
    treeTexture.loadFromFile("graphics/tree.png");

    Sprite treeSprite;
    treeSprite.setTexture(treeTexture);
    treeSprite.setPosition(510,0);

    Texture cloudTexture;
    cloudTexture.loadFromFile("graphics/cloud.png");

    Sprite cloudSprite1;
    cloudSprite1.setTexture(cloudTexture);
    cloudSprite1.setPosition(0,0);

    Sprite cloudSprite2;
    cloudSprite2.setTexture(cloudTexture);
    cloudSprite2.setPosition(0,150);

    Sprite cloudSprite3;
    cloudSprite3.setTexture(cloudTexture);
    cloudSprite3.setPosition(200,70);

    Texture beeTexture;
    beeTexture.loadFromFile("graphics/bee.png");

    Sprite beeSprite1;
    beeSprite1.setTexture(beeTexture);
    beeSprite1.setPosition(1200,400);

    // Sprite beeSprite2;
    // beeSprite2.setTexture(beeTexture);
    // beeSprite2.setPosition(200,600);

    //NOTES-050325
    // for bee movements
    //1. dt-delta value
    //2.clock
    //3.position and speed
    //4.generate random value- rand()

    bool cloudActive1=false;
    float cloudSpeed1=0.0f;
    float cloudHeight1=0.0f;

    bool cloudActive2=false;
    float cloudSpeed2=0.0f;
    float cloudHeight2=0.0f;

    bool cloudActive3=false;
    float cloudSpeed3=0.0f;
    float cloudHeight3=0.0f;


    bool beeActive1=false;
    float beeSpeed1=0.0f;
    float beeHeight1=0.0f;

    // bool beeActive2=false;
    // float beeSpeed2=0.0f;
    // float beeHeight2=0.0f;

    Clock clock;

    //time Bar- width reduces, height fixed.
    RectangleShape timeBar;
    float timeBarStartWidth=400;
    float timeBarHeight=40;
    timeBar.setSize(Vector2f(timeBarStartWidth,timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1300/2)-timeBarStartWidth/2,600);

    //time gameTime total
    float timeRemaining=6.0f;
    float timeBarWidthPerSecond=timeBarStartWidth/timeRemaining;

    //track game is running or not
    bool paused=true;

    //draw score text
    int score=0;
    sf::Text messageText;
    sf::Text scoreText;

    sf::Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    //set font to our message
    messageText.setFont(font);
    scoreText.setFont(font);
    //assign actual msg
    messageText.setString("Press ENTER to start");
    scoreText.setString("Score: 0");
    //set size
    messageText.setCharacterSize(50);
    scoreText.setCharacterSize(50);
    //set color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    //position of text
    FloatRect textRect=messageText.getLocalBounds();//depending on local bound set origin changes

    //imp
    messageText.setOrigin(textRect.left+textRect.width/2.0f,textRect.top+textRect.height/2.0f);


    //set position
    messageText.setPosition(700,400);

    //messageText.setPosition(1920/2.0f,1080/2.0f);
    scoreText.setPosition(30,30);


    // //draw message text
    // sf::Text msgText;
    // //set font to our message
    // msgText.setFont(font);
    // //assign actual msg
    // msgText.setString("This is a MSG");
    // //set size
    // msgText.setCharacterSize(100);
    // //set color
    // msgText.setFillColor(Color::Blue);
    // //set position
    // msgText.setPosition(100,90);

    //prepare 5 branches
    Texture textureBranch; //adding sound effects
    textureBranch.loadFromFile("graphics/branch.png");

    //set texture for each branch sprite
    for(int i=0;i<NUM_BRANCHES;i++){
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000,-2000);//default values -2000,-2000
        //set the sprite origin
        branches[i].setOrigin(220,20);//default values 220,20
    }

    // updateBranches(1);
    // updateBranches(2);
    // updateBranches(3);
    // updateBranches(4);
    // updateBranches(5);
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(350, 520);

    // The player starts on the left
    side playerSide = side::LEFT;

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(675, 2000);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(350, 560);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 350;
    const float AXE_POSITION_RIGHT = 850;

    // Prepare the flying log //adding sound effects

    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(510, 560);
     // Some other useful log related variables
     bool logActive = false;
     float logSpeedX = 1000;
     float logSpeedY = -1500;
      // Control the player input
    bool acceptInput = false;

  //Sounds
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer outBuffer;
	outBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(outBuffer);


    //main loop
    while(window.isOpen())//used for continuous reading
    {   
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                // Listen for key presses again
                acceptInput = true;

                // hide the axe
                spriteAxe.setPosition(2000,
                    spriteAxe.getPosition().y);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        //start the game
        if(Keyboard::isKeyPressed(Keyboard::Space))
        {
            paused=false;
            //reset time
            score=0;
            timeRemaining=6.0f;
            // Make all the branches disappear -
            // starting in the second position
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            // Make sure the gravestone is hidden
            spriteRIP.setPosition(675, 2000);

            // Move the player into position
            spritePlayer.setPosition(675, 660);

            acceptInput = true;

        }
        // Wrap the player controls to
        // Make sure we are accepting input
        if (acceptInput)
        {
            // First handle pressing the right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                // Make sure the player is on the right
                playerSide = side::RIGHT;
                score++;

                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;

                spritePlayer.setPosition(850, 520);
                
                spriteAxe.setPosition(AXE_POSITION_RIGHT,spriteAxe.getPosition().y);
                
                
                // Set the log flying to the left
                spriteLog.setPosition(510, 560);
                logSpeedX = -1000;
                logActive = true;
                
                // Update the branches
                updateBranches(score);
                
                acceptInput = false;

            }
        
        // Handle the left cursor key
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            // Make sure the player is on the left
            playerSide = side::LEFT;
            score++;

            // Add to the amount of time remaining
            timeRemaining += (2 / score) + .15;

            spriteAxe.setPosition(AXE_POSITION_LEFT,spriteAxe.getPosition().y);

            spritePlayer.setPosition(350, 520);

            // update the branches
            updateBranches(score);

            // set the log flying
            spriteLog.setPosition(510, 560);
            logSpeedX = 1000;
            logSpeedY = -1500;

            acceptInput = false;

        }
    }
        //update the screen
    if(!paused)//not paused
    {
            //Measure the time & update the scene
            Time dt= clock.restart();
            //substract from the amount of time remaining
            timeRemaining-=dt.asSeconds();//delta time calculation
            //sizeup the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond*timeRemaining,timeBarHeight));
            if(timeRemaining<0.0f)
            {
                paused=true;
                //change the message shown to the player
                messageText.setString("Game Over!");
                //Reposotion the text based on new size
                FloatRect textRect=messageText.getLocalBounds();
                messageText.setOrigin(textRect.left+textRect.width/2.0f,textRect.top+textRect.height/2.0f);
                //messageText.setPosition(1920/2.0f,1080/2.0f);

                messageText.setPosition(700,400);
            }
        

        if(!cloudActive1){
            //set the cloud1 speed, height and position
            srand((int)time(0)*10);
            cloudSpeed1=(rand()%200)+200;//speed 0 to +200 means 1 to 199 pixels per second

            srand((int)time(0)*20);
            cloudHeight1=(rand()%300)+100;

            cloudSprite1.setPosition(-200,cloudHeight1);
            cloudActive1=true;
        }
        else{
            cloudSprite1.setPosition(cloudSprite1.getPosition().x +cloudSpeed1*dt.asSeconds(),cloudSprite1.getPosition().y);//-beespeed becoz bee is moving from right to left
            if(cloudSprite1.getPosition().x>1920){
                cloudActive1=false;
            }
        }



        if(!cloudActive2){
            srand((int)time(0)*10);
            cloudSpeed2=(rand()%200)+250;

            srand((int)time(0)*20);
            cloudHeight2=(rand()%300-150)+100;

            cloudSprite2.setPosition(-200,cloudHeight2);
            cloudActive2=true;
        }
        else{
            cloudSprite2.setPosition(cloudSprite2.getPosition().x +cloudSpeed2*dt.asSeconds(),cloudSprite2.getPosition().y);//-beespeed becoz bee is moving from right to left
            if(cloudSprite2.getPosition().x>1920){
                cloudActive2=false;
            }
        }

        if(!cloudActive3){
            srand((int)time(0)*10);
            cloudSpeed3=(rand()%200)+300;

            srand((int)time(0)*20);
            cloudHeight1=(rand()%450-150)+100;

            cloudSprite3.setPosition(-200,cloudHeight3);
            cloudActive3=true;
        }
        else{
            cloudSprite3.setPosition(cloudSprite3.getPosition().x +cloudSpeed3*dt.asSeconds(),cloudSprite3.getPosition().y);//-beespeed becoz bee is moving from right to left
            if(cloudSprite3.getPosition().x>1920){
                cloudActive3=false;
            }
        }
        //handling bee
        if(!beeActive1){
            //set the bee speed, height and position
            srand((int)time(0)*10);
            beeSpeed1=(rand()%200)+200;//speed 0 to +200 means 1 to 199 pixels per second

            srand((int)time(0)*20);
            beeHeight1=(rand()%500)+100;

            beeSprite1.setPosition(2000,beeHeight1);
            beeActive1=true;
        }
        else{
            beeSprite1.setPosition(beeSprite1.getPosition().x -beeSpeed1*dt.asSeconds(),beeSprite1.getPosition().y);//-beespeed becoz bee is moving from right to left
            if(beeSprite1.getPosition().x<-100){
                beeActive1=false;
            }
        }
        // if(!beeActive2){
        //     //set the bee speed, height and position
        //     srand((int)time(0)*10);
        //     beeSpeed2=(rand()%200)+200;//speed 0 to +200 means 1 to 199 pixels per second

        //     srand((int)time(0)*20);
        //     beeHeight2=(rand()%500)+100;

        //     beeSprite2.setPosition(-100,beeHeight2);
        //     beeActive2=true;


        // }
        // else{
        //     beeSprite2.setPosition(beeSprite2.getPosition().x +beeSpeed2*dt.asSeconds(),beeSprite2.getPosition().y);//-beespeed becoz bee is moving from right to left
        //     if(beeSprite2.getPosition().x>1920){
        //         beeActive2=false;
        //     }
        // }



        //update score
        std::stringstream ss;
        ss << "Score: " << score;
        scoreText.setString(ss.str());

        //update the branch sprite
        for(int i=0;i<NUM_BRANCHES;i++){
            float height=i*130;
            if(branchPositions[i]==side::LEFT)
            {
                //move the sprite to the left side
                branches[i].setPosition(510,height);//510
                //flip the sprite round the other way
                branches[i].setRotation(180);

            }
            else if(branchPositions[i]==side::RIGHT){
                //move the sprite to the right side
                branches[i].setPosition(1030,height);//1330
                //flip the sprite round the other way
                branches[i].setRotation(0);
            }
            else{
                //hide the branch
                branches[i].setPosition(3000,height);
            }
            
        }
        // Handle a flying log
        if (logActive)
        {
            spriteLog.setPosition(
                spriteLog.getPosition().x +
                (logSpeedX * dt.asSeconds()),
                spriteLog.getPosition().y +
                (logSpeedY * dt.asSeconds())
            );

            // Has the log reached the right hand edge?
            if (spriteLog.getPosition().x < -100 ||
                spriteLog.getPosition().x > 1500)
            {
                // Set it up ready to be a whole new log next frame
                logActive = false;
                spriteLog.setPosition(500, 580);
            }
        }
        if (branchPositions[5] == playerSide)
        {
            // Death
            paused = true;
            acceptInput = false;

            // Draw the gravestone
            spriteRIP.setPosition(525, 550);

            // Hide the player
            spritePlayer.setPosition(2000, 660);

            // Change the text of the message
            messageText.setString("SQUISHED!");

            // Center it on the screen
            FloatRect textRect = messageText.getLocalBounds();

            messageText.setOrigin(textRect.left +
                textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);

            messageText.setPosition(1336 / 2.0f,
                768 / 2.0f);

            // Play the death sound
           
        }

    }//pause if bracket

        



        //clear window
        window.clear();
        //display- layer wise display
        window.draw(backgroundSprite);
        window.draw(cloudSprite1);
        window.draw(cloudSprite2);
        window.draw(cloudSprite3);
        
        
        //draw the branches
        for(int i=0;i<NUM_BRANCHES;i++){
            window.draw(branches[i]);
        }
        window.draw(treeSprite);
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);
        window.draw(beeSprite1);
        //window.draw(beeSprite2);
        window.draw(scoreText);
        //window.draw(msgText);
        window.draw(timeBar);
        if(paused)
        {
            //Draw our message
            window.draw(messageText);
        }
        window.display();

    }
    return 0;
}

//function definition
void updateBranches(int seed)
{
    //MOVE ALL BRANCHES DOWN ONE PLACE
    for(int j=NUM_BRANCHES-1;j>0;j--){
        branchPositions[j]=branchPositions[j-1];

    }
    //LEFT RIGHT or NONE
    srand((int)time(0)+seed);
    int r=(rand()%5);
    switch (r)
    {
    case 0:
    branchPositions[0]=side::LEFT;
        break;
    case 1:
    branchPositions[0]=side::RIGHT;
        break;
    default:
    branchPositions[0]=side::NONE;
        break;
    }
}



//g++ Timber.cpp -o a -lsfml-graphics -lsfml-window -lsfml-system

//g++ Timber.cpp -o a -lsfml-graphics -lsfml-window -lsfml-system
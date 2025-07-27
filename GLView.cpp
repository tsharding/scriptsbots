#include "GLView.h"
#include "StatsWindow.h"

#include <GL/glut.h>

#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

void gl_processNormalKeys(unsigned char key, int x, int y)
{
    GLVIEW->processNormalKeys(key, x, y);
}
void gl_changeSize(int w, int h)
{
    GLVIEW->changeSize(w,h);
}
void gl_handleIdle()
{
    GLVIEW->handleIdle();
}
void gl_processMouse(int button, int state, int x, int y)
{
    GLVIEW->processMouse(button, state, x, y);
}
void gl_processMouseActiveMotion(int x, int y)
{
    GLVIEW->processMouseActiveMotion(x,y);
}
void gl_renderScene()
{
    // Only render if this is the main window
    if (glutGetWindow() == 1) {
        GLVIEW->renderScene();
    }
}


void RenderString(float x, float y, void *font, const char* string, float r, float g, float b)
{
    glColor3f(r,g,b);
    glRasterPos2f(x, y);
    int len = (int) strlen(string);
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
}

void drawCircle(float x, float y, float r) {
    float n;
    for (int k=0;k<17;k++) {
        n = k*(M_PI/8);
        glVertex3f(x+r*sin(n),y+r*cos(n),0);
    }
}


GLView::GLView(World *s) :
        world(s),
        paused(false),
        draw(true),
        skipdraw(1),
        drawfood(true),
        showAgentInfo(true),
        modcounter(0),
        frames(0),
        lastUpdate(0),
        windowWidth(conf::WWIDTH()),
        windowHeight(conf::WHEIGHT())
{

    // Calculate initial zoom to fit world in window
    float worldAspectRatio = (float)conf::WIDTH() / conf::HEIGHT();
    float windowAspectRatio = (float)windowWidth / windowHeight;
    
    if (worldAspectRatio > windowAspectRatio) {
        // World is wider than window - fit to width
        scalemult = (float)windowWidth / conf::WIDTH();
    } else {
        // World is taller than window - fit to height
        scalemult = (float)windowHeight / conf::HEIGHT();
    }
    
    // Center the world in the window
    xtranslate = -conf::WIDTH() / 2.0f;
    ytranslate = -conf::HEIGHT() / 2.0f;
    
    downb[0]=0;downb[1]=0;downb[2]=0;
    mousex=0;mousey=0;
    
    following = false;
}

GLView::~GLView()
{

}
void GLView::changeSize(int w, int h)
{
    // Store the new window dimensions
    windowWidth = w;
    windowHeight = h;
    
    // Set the viewport to the new window size
    glViewport(0, 0, w, h);
    
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0, 1);
    
    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLView::processMouse(int button, int state, int x, int y)
{
    //printf("MOUSE EVENT: button=%i state=%i x=%i y=%i\n", button, state, x, y);
    
    // Handle mouse wheel events (button 3 = wheel up, button 4 = wheel down)
    if (button == 3 || button == 4) {
        float zoomFactor = 0.03f; // How much to zoom per wheel click (reduced sensitivity)
        float oldScale = scalemult;
        
        if (button == 3) {
            // Wheel up - zoom in
            scalemult += zoomFactor;
        } else {
            // Wheel down - zoom out
            scalemult -= zoomFactor;
        }
        
        // Clamp zoom to reasonable limits
        if (scalemult < 0.01f) scalemult = 0.01f;
        if (scalemult > 5.0f) scalemult = 5.0f;
        
        // Adjust translation to zoom towards mouse cursor
        float mouseXWorld = (x - windowWidth/2) / oldScale - xtranslate;
        float mouseYWorld = (y - windowHeight/2) / oldScale - ytranslate;
        
        xtranslate = (x - windowWidth/2) / scalemult - mouseXWorld;
        ytranslate = (y - windowHeight/2) / scalemult - mouseYWorld;
        
        mousex = x; mousey = y;
        return;
    }
    
    //have world deal with it. First translate to world coordinates though
    if(button==0){
            int wx= (int) ((x-windowWidth/2)/scalemult)-xtranslate;
    int wy= (int) ((y-windowHeight/2)/scalemult)-ytranslate;
        world->processMouse(button, state, wx, wy);
    }
    
    mousex=x; mousey=y;
    downb[button]=1-state; //state is backwards, ah well
}

void GLView::processMouseActiveMotion(int x, int y)
{
    //printf("MOUSE MOTION x=%i y=%i, %i %i %i\n", x, y, downb[0], downb[1], downb[2]);
    
    if(downb[1]==1){
        //mouse wheel. Change scale
        scalemult -= 0.002*(y-mousey);
        if(scalemult<0.01) scalemult=0.01;
    }
    
    if(downb[2]==1){
        //right mouse button. Pan around
        // Scale translation by inverse of zoom to make displacement follow mouse exactly
        xtranslate += (x-mousex) / scalemult;
        ytranslate += (y-mousey) / scalemult;
    }
    
//    printf("%f %f %f \n", scalemult, xtranslate, ytranslate);
    
    mousex=x;
    mousey=y;
}

void GLView::processNormalKeys(unsigned char key, int x, int y)
{
    // Process keys regardless of which window has focus
    // (this allows stats window to forward keys to main window)

    if (key == 27)
        exit(0);
    else if (key=='r') {
        world->reset();
        printf("Agents reset\n");
    } else if (key=='p') {
        //pause
        paused= !paused;
    } else if (key=='d') {
        //drawing
        draw= !draw;
    } else if (key==43) {
        //+
        skipdraw++;

    } else if (key==45) {
        //-
        skipdraw--;
    } else if (key=='f') {
        drawfood=!drawfood;
    } else if (key=='g') {
        showAgentInfo=!showAgentInfo;
    } else if (key=='v') {
        // Save simulation state
        std::string saveName = "manual_save_" + std::to_string(world->epoch()) + ".sav";
        if (world->saveToFile(saveName)) {
            printf("Manual save successful: %s\n", saveName.c_str());
        } else {
            printf("Manual save failed!\n");
        }
    } else if (key=='l') {
        // Load simulation state - find and load the most recent save file
        bool loaded = false;
        
        // Function to find the most recent save file
        std::string saveDir = world->getSaveDirectory();
        auto findMostRecentSave = [saveDir](const std::string& prefix) -> std::string {
            std::string mostRecentFile = "";
            time_t mostRecentTime = 0;
            
            // Check files from epoch 0 to 1000 (should be enough)
            for (int epoch = 0; epoch <= 1000; epoch++) {
                std::string filename = saveDir + prefix + std::to_string(epoch) + ".sav";
                FILE* file = fopen(filename.c_str(), "rb");
                if (file) {
                    // Get file modification time
                    struct stat fileStat;
                    if (stat(filename.c_str(), &fileStat) == 0) {
                        if (fileStat.st_mtime > mostRecentTime) {
                            mostRecentTime = fileStat.st_mtime;
                            mostRecentFile = prefix + std::to_string(epoch) + ".sav";
                        }
                    }
                    fclose(file);
                }
            }
            return mostRecentFile;
        };
        
        // Find the most recent save file across all types
        std::string mostRecentFile = "";
        time_t mostRecentTime = 0;
        
        // Check manual saves
        std::string mostRecentManual = findMostRecentSave("manual_save_");
        if (!mostRecentManual.empty()) {
            mostRecentFile = mostRecentManual;
        }
        
        // Check autosaves and compare timestamps
        std::string mostRecentAuto = findMostRecentSave("autosave_epoch_");
        if (!mostRecentAuto.empty()) {
            // Compare timestamps by checking the actual files
            std::string manualPath = saveDir + mostRecentManual;
            std::string autoPath = saveDir + mostRecentAuto;
            
            struct stat manualStat, autoStat;
            bool manualExists = (stat(manualPath.c_str(), &manualStat) == 0);
            bool autoExists = (stat(autoPath.c_str(), &autoStat) == 0);
            
            if (manualExists && autoExists) {
                if (autoStat.st_mtime > manualStat.st_mtime) {
                    mostRecentFile = mostRecentAuto;
                }
            } else if (autoExists) {
                mostRecentFile = mostRecentAuto;
            }
        }
        
        // Load the most recent file found
        if (!mostRecentFile.empty()) {
            if (world->loadFromFile(mostRecentFile)) {
                printf("Load successful: %s\n", mostRecentFile.c_str());
                loaded = true;
            }
        }
        
        if (!loaded) {
            printf("No save files found to load!\n");
        }
    } else if (key=='a') {
        for (int i=0;i<10;i++){world->addNewByCrossover();}
    } else if (key=='q') {
        for (int i=0;i<10;i++){world->addCarnivore();}
    } else if (key=='h') {
        for (int i=0;i<10;i++){world->addHerbivore();}
    } else if (key=='c') {
        world->setClosed( !world->isClosed() );
        printf("Environment closed now= %i\n",world->isClosed());
    } else if (key=='s') {
        if(following==0) following=2;
        else following=0;
    } else if(key =='o') {
        if(following==0) following = 1; //follow oldest agent: toggle
        else following =0;
    } else if(key =='j' || key =='J') {
        // Recenter the camera view
        following = 0; // Stop following any agent
        xtranslate = -conf::WIDTH() / 2.0f;
        ytranslate = -conf::HEIGHT() / 2.0f;
        printf("Camera recentered\n");
    } else {
        printf("Unknown key pressed: %i\n", key);
    }
}

void GLView::handleIdle()
{
    // Always update the world simulation regardless of window focus
    modcounter++;
    if (!paused) world->update();

    //show FPS
    int currentTime = glutGet( GLUT_ELAPSED_TIME );
    frames++;
    if ((currentTime - lastUpdate) >= 1000) {
        std::pair<int,int> num_herbs_carns = world->numHerbCarnivores();
        sprintf( buf, "ScriptBots - World View (FPS: %d)", frames );
        glutSetWindowTitle( buf );
        frames = 0;
        lastUpdate = currentTime;
        
        // Update stats window only when FPS updates (once per second)
        if (STATSWINDOW) {
            STATSWINDOW->updateDisplay();
        }
    }
    if (skipdraw<=0 && draw) {
        clock_t endwait;
        float mult=-0.005*(skipdraw-1); //ugly, ah well
        endwait = clock () + mult * CLOCKS_PER_SEC ;
        while (clock() < endwait) {}
    }

    // Always render the main window if drawing is enabled
    if (draw) {
        int currentWindow = glutGetWindow();
        glutSetWindow(1); // Ensure we're rendering to main window
        if (skipdraw>0) {
            if (modcounter%skipdraw==0) renderScene();    //increase fps by skipping drawing
        }
        else renderScene(); //we will decrease fps by waiting using clocks
        if (currentWindow != 1) {
            glutSetWindow(currentWindow); // Restore previous window context
        }
    }
}

void GLView::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glTranslatef(windowWidth/2, windowHeight/2, 0.0f);    
    glScalef(scalemult, scalemult, 1.0f);
    
    if(following==0) {
        glTranslatef(xtranslate, ytranslate, 0.0f);    
    } else {
        
        float xi=0, yi=0;
        world->positionOfInterest(following, xi, yi);
        //xi= (windowWidth/2-xi); //*scalemult;
        //yi= (windowHeight/2-yi); //*scalemult;
        
        glTranslatef(-xi, -yi, 0.0f);
        
        //reset this if there is no interest. Probably agent that was followed died
        if(xi==0 && yi==0) following = 0;
    }
    
    world->draw(this, drawfood);

    glPopMatrix();
    glutSwapBuffers();
}

void GLView::drawAgent(const Agent& agent)
{
    float n;
    float r= conf::BOTRADIUS();
    float rp= conf::BOTRADIUS()+2;
    //handle selected agent
    if (agent.selectflag>0) {

        //draw selection
        glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        drawCircle(agent.pos.x, agent.pos.y, conf::BOTRADIUS()+5);
        glEnd();

        glPushMatrix();
        glTranslatef(agent.pos.x-80,agent.pos.y+20,0);
        //draw inputs, outputs
        float col;
        float yy=15;
        float xx=15;
        float ss=16;
        glBegin(GL_QUADS);
        for (int j=0;j<conf::INPUTSIZE();j++) {
            col= agent.in[j];
            glColor3f(col,col,col);
            glVertex3f(0+ss*j, 0, 0.0f);
            glVertex3f(xx+ss*j, 0, 0.0f);
            glVertex3f(xx+ss*j, yy, 0.0f);
            glVertex3f(0+ss*j, yy, 0.0f);
        }
        yy+=5;
        for (int j=0;j<conf::OUTPUTSIZE();j++) {
            col= agent.out[j];
            glColor3f(col,col,col);
            glVertex3f(0+ss*j, yy, 0.0f);
            glVertex3f(xx+ss*j, yy, 0.0f);
            glVertex3f(xx+ss*j, yy+ss, 0.0f);
            glVertex3f(0+ss*j, yy+ss, 0.0f);
        }
        yy+=ss*2;

        //draw brain. Eventually move this to brain class?
        
        float offx=0;
        ss=8;
        xx=ss;
        for (int j=0;j<conf::BRAINSIZE();j++) {
            col = agent.brain.boxes[j].out;
            glColor3f(col,col,col);
            
            glVertex3f(offx+0+ss*j, yy, 0.0f);
            glVertex3f(offx+xx+ss*j, yy, 0.0f);
            glVertex3f(offx+xx+ss*j, yy+ss, 0.0f);
            glVertex3f(offx+ss*j, yy+ss, 0.0f);
            
            if ((j+1)%30==0) {
                yy+=ss;
                offx-=ss*30;
            }
        }
        
        
        /*
        glEnd();
        glBegin(GL_LINES);
        float offx=0;
        ss=30;
        xx=ss;
        for (int j=0;j<conf::BRAINSIZE();j++) {
            for(int k=0;k<conf::CONNS();k++){
                int j2= agent.brain.boxes[j].id[k];
                
                //project indices j and j2 into pixel space
                float x1= 0;
                float y1= 0;
                if(j<conf::INPUTSIZE()) { x1= j*ss; y1= yy; }
                else { 
                    x1= ((j-conf::INPUTSIZE())%30)*ss;
                    y1= yy+ss+2*ss*((int) (j-conf::INPUTSIZE())/30);
                }
                
                float x2= 0;
                float y2= 0;
                if(j2<conf::INPUTSIZE()) { x2= j2*ss; y2= yy; }
                else { 
                    x2= ((j2-conf::INPUTSIZE())%30)*ss;
                    y2= yy+ss+2*ss*((int) (j2-conf::INPUTSIZE())/30);
                }
                
                float ww= agent.brain.boxes[j].w[k];
                if(ww<0) glColor3f(-ww, 0, 0);
                else glColor3f(0,0,ww);
                
                glVertex3f(x1,y1,0);
                glVertex3f(x2,y2,0);
            }
        }
        */

        glEnd();
        glPopMatrix();
    }

    //draw giving/receiving
    if(agent.dfood!=0){
        glBegin(GL_POLYGON);
        float mag=cap(abs(agent.dfood)/conf::FOODTRANSFER()/3);
        if(agent.dfood>0) glColor3f(0,mag,0); //draw boost as green outline
        else glColor3f(mag,0,0);
        for (int k=0;k<17;k++){
            n = k*(M_PI/8);
            glVertex3f(agent.pos.x+rp*sin(n),agent.pos.y+rp*cos(n),0);
            n = (k+1)*(M_PI/8);
            glVertex3f(agent.pos.x+rp*sin(n),agent.pos.y+rp*cos(n),0);
        }
        glEnd();
    }

    //draw indicator of this agent... used for various events
     if (agent.indicator>0) {
         glBegin(GL_POLYGON);
         glColor3f(agent.ir,agent.ig,agent.ib);
         drawCircle(agent.pos.x, agent.pos.y, conf::BOTRADIUS()+((int)agent.indicator));
         glEnd();
     }
    
    
    //draw eyes
    glBegin(GL_LINES);
    glColor3f(0.5,0.5,0.5);
    for(int q=0;q<conf::NUMEYES();q++) {
        glVertex3f(agent.pos.x,agent.pos.y,0);
//        float aa= agent.angle+agent.eyedir[q]+agent.eyefov[q];
        float aa= agent.angle+agent.eyedir[q];
        glVertex3f(agent.pos.x+(conf::BOTRADIUS()*4)*cos(aa),
                   agent.pos.y+(conf::BOTRADIUS()*4)*sin(aa),
                   0);
        //aa = agent.angle+agent.eyedir[q]-agent.eyefov[q];
        //glVertex3f(agent.pos.x,agent.pos.y,0);
        //glVertex3f(agent.pos.x+(conf::BOTRADIUS*4)*cos(aa),
        //           agent.pos.y+(conf::BOTRADIUS*4)*sin(aa),
        //           0);
    }
    glEnd();
    
    glBegin(GL_POLYGON); //body
    glColor3f(agent.red,agent.gre,agent.blu);
    drawCircle(agent.pos.x, agent.pos.y, conf::BOTRADIUS());
    glEnd();

    glBegin(GL_LINES);
    //outline
    if (agent.boost) glColor3f(0.8,0,0); //draw boost as green outline
    else glColor3f(0,0,0);

    for (int k=0;k<17;k++)
    {
        n = k*(M_PI/8);
        glVertex3f(agent.pos.x+r*sin(n),agent.pos.y+r*cos(n),0);
        n = (k+1)*(M_PI/8);
        glVertex3f(agent.pos.x+r*sin(n),agent.pos.y+r*cos(n),0);
    }
    //and spike
    glColor3f(0.5,0,0);
    glVertex3f(agent.pos.x,agent.pos.y,0);
    glVertex3f(agent.pos.x+(3*r*agent.spikeLength)*cos(agent.angle),agent.pos.y+(3*r*agent.spikeLength)*sin(agent.angle),0);
    glEnd();

    //and health bars (only if showAgentInfo is enabled)
    if (showAgentInfo) {
        int xo=18;
        int yo=-15;
        glBegin(GL_QUADS);
        //black background
        glColor3f(0,0,0);
        glVertex3f(agent.pos.x+xo,agent.pos.y+yo,0);
        glVertex3f(agent.pos.x+xo+5,agent.pos.y+yo,0);
        glVertex3f(agent.pos.x+xo+5,agent.pos.y+yo+40,0);
        glVertex3f(agent.pos.x+xo,agent.pos.y+yo+40,0);

        //health
        glColor3f(0,0.8,0);
        glVertex3f(agent.pos.x+xo,agent.pos.y+yo+20*(2-agent.health),0);
        glVertex3f(agent.pos.x+xo+5,agent.pos.y+yo+20*(2-agent.health),0);
        glVertex3f(agent.pos.x+xo+5,agent.pos.y+yo+40,0);
        glVertex3f(agent.pos.x+xo,agent.pos.y+yo+40,0);

        //if this is a hybrid, we want to put a marker down
        if (agent.hybrid) {
            glColor3f(0,0,0.8);
            glVertex3f(agent.pos.x+xo+6,agent.pos.y+yo,0);
            glVertex3f(agent.pos.x+xo+12,agent.pos.y+yo,0);
            glVertex3f(agent.pos.x+xo+12,agent.pos.y+yo+10,0);
            glVertex3f(agent.pos.x+xo+6,agent.pos.y+yo+10,0);
        }

        glColor3f(1-agent.herbivore,agent.herbivore,0);
        glVertex3f(agent.pos.x+xo+6,agent.pos.y+yo+12,0);
        glVertex3f(agent.pos.x+xo+12,agent.pos.y+yo+12,0);
        glVertex3f(agent.pos.x+xo+12,agent.pos.y+yo+22,0);
        glVertex3f(agent.pos.x+xo+6,agent.pos.y+yo+22,0);

        //how much sound is this bot making?
        glColor3f(agent.soundmul,agent.soundmul,agent.soundmul);
        glVertex3f(agent.pos.x+xo+6,agent.pos.y+yo+24,0);
        glVertex3f(agent.pos.x+xo+12,agent.pos.y+yo+24,0);
        glVertex3f(agent.pos.x+xo+12,agent.pos.y+yo+34,0);
        glVertex3f(agent.pos.x+xo+6,agent.pos.y+yo+34,0);

        //draw giving/receiving
        if (agent.dfood!=0) {

            float mag=cap(abs(agent.dfood)/conf::FOODTRANSFER()/3);
            if (agent.dfood>0) glColor3f(0,mag,0); //draw boost as green outline
            else glColor3f(mag,0,0);
            glVertex3f(agent.pos.x+xo+6,agent.pos.y+yo+36,0);
            glVertex3f(agent.pos.x+xo+12,agent.pos.y+yo+36,0);
            glVertex3f(agent.pos.x+xo+12,agent.pos.y+yo+46,0);
            glVertex3f(agent.pos.x+xo+6,agent.pos.y+yo+46,0);
        }


        glEnd();
    }

    //print stats (only if showAgentInfo is enabled)
    if (showAgentInfo) {
        //generation count
        sprintf(buf2, "%i", agent.gencount);
        RenderString(agent.pos.x-conf::BOTRADIUS()*1.5, agent.pos.y+conf::BOTRADIUS()*1.8, GLUT_BITMAP_TIMES_ROMAN_24, buf2, 0.0f, 0.0f, 0.0f);
        //age
        sprintf(buf2, "%i", agent.age);
        float x = agent.age/1000.0;
        if(x>1)x=1;
        RenderString(agent.pos.x-conf::BOTRADIUS()*1.5, agent.pos.y+conf::BOTRADIUS()*1.8+12, GLUT_BITMAP_TIMES_ROMAN_24, buf2, x, 0.0f, 0.0f);

        //health
        sprintf(buf2, "%.2f", agent.health);
        RenderString(agent.pos.x-conf::BOTRADIUS()*1.5, agent.pos.y+conf::BOTRADIUS()*1.8+24, GLUT_BITMAP_TIMES_ROMAN_24, buf2, 0.0f, 0.0f, 0.0f);

        //repcounter
        sprintf(buf2, "%.2f", agent.repcounter);
        RenderString(agent.pos.x-conf::BOTRADIUS()*1.5, agent.pos.y+conf::BOTRADIUS()*1.8+36, GLUT_BITMAP_TIMES_ROMAN_24, buf2, 0.0f, 0.0f, 0.0f);
        
        //lineage tag
        RenderString(agent.pos.x-conf::BOTRADIUS()*1.5, agent.pos.y+conf::BOTRADIUS()*1.8+48, GLUT_BITMAP_TIMES_ROMAN_24, agent.lineageTag.c_str(), 0.0f, 0.0f, 0.0f);
    }
}

void GLView::drawMisc()
{
    // Population chart and controls info moved to separate stats window
    // This function is now empty to keep the interface clean
}

void GLView::drawFood(int x, int y, float quantity)
{
    //draw food
    if (drawfood) {
        glBegin(GL_QUADS);
        glColor3f(0.9-quantity,0.9-quantity,1.0-quantity);
        glVertex3f(x*conf::CZ(),y*conf::CZ(),0);
        glVertex3f(x*conf::CZ()+conf::CZ(),y*conf::CZ(),0);
        glVertex3f(x*conf::CZ()+conf::CZ(),y*conf::CZ()+conf::CZ(),0);
        glVertex3f(x*conf::CZ(),y*conf::CZ()+conf::CZ(),0);
        glEnd();
    }
}

void GLView::setWorld(World* w)
{
    world = w;
}

//Modified by: Karen Salinas
//Modified Date: 6/24/2017
//Week 4
//On this program, I am in charge of the background.
//===============================================================
//Week 8
//Changed made: Added code for tiles, still need to edit
//
//===============================================================
//Week 9
//Changes made: Fixed parallax scrolling
//              Added the ground by rendering it
//              Tiles are currently being fixed
//===============================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <cmath>
#include "log.h"
#include "ppm.h"
#include "fonts.h"
#include <unistd.h> //added to solve error for close(sock);
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <ctime>
#include <string>
#include <sstream>
#include "game.h"
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>

#define HOST "www.google.com"
#define PAGE "/"
#define PORT 80
#define USERAGENT "HTMLGET 1.0"

using namespace std;

#include <iostream>

class SpriteWrapAround : public Sprite {
public:

    SpriteWrapAround(
            const std::string & filename,
            float height,
            float width) : Sprite(filename, 1, 1, 1, 1, height, width)
    {
    }

    void draw()
    {
        //Calculate the sprite frame and size
        //and location
        float camX = -gl.camera[0];
        float cx = posX;
        float cy = posY;
        float h2 = height / 2.0;
        float repeatX = (camX - cx) / gl.xres;
        float xc0 = 0 + repeatX;
        float xc1 = (float) gl.xres / origWidth + repeatX;

        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, glTexture);

        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255, 255, 255, 255);

        glBegin(GL_QUADS);
        glTexCoord2f(xc0, 1);
        glVertex2i(camX, cy - h2);
        glTexCoord2f(xc0, 0);
        glVertex2i(camX, cy + h2);
        glTexCoord2f(xc1, 0);
        glVertex2i(gl.xres + camX, cy + h2);
        glTexCoord2f(xc1, 1);
        glVertex2i(gl.xres + camX, cy - h2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }
};

void initBackgroundSprites()
{
    globalSprite.background[0] = new Sprite("bg.png", 1, 1, 1, 1, 1200, 5000);
    globalSprite.background[1] = new Sprite("moon.png", 1, 1, 1, 1, 100, 100);
    globalSprite.background[1]->setPos(5000 / 2, 0.8 * gl.yres);
    globalSprite.background[2] = new SpriteWrapAround("mountain-fg.png", 513, 
    5000);
    globalSprite.background[2]->setPos(5000 / 2, 360);
    globalSprite.background[3] = new SpriteWrapAround("mountain-bg.png", 703, 
    5000);
    globalSprite.background[3]->setPos(5000 / 2, 360);
    globalSprite.background[4] = new SpriteWrapAround("treeline.png", 770, 
    5000);
    globalSprite.background[4]->setPos(5000 / 2, 200);

}

void renderBackgroundSprites()
{
    for (int i = 0; i < 5; i++) {
        if (i == 0) {
            globalSprite.background[i]->setPos(-gl.camera[0], 600);
        } else if (i == 1) {
            globalSprite.background[i]->setPos(-gl.camera[0] + gl.xres * 0.66, 
                    gl.yres * 0.8);
        } else if (i == 2) {
            globalSprite.background[i]->setPos(-gl.camera[0]*0.1, 360);
        } else if (i == 3) {
            globalSprite.background[i]->setPos(-gl.camera[0]*0.50, 360);
        } else {
            globalSprite.background[i]->setPos(gl.camera[0], 250);
        }
        globalSprite.background[i]->draw();
    }

    glColor3ub(24, 24, 24);
    glBegin(GL_QUADS);
    glVertex2i(-gl.camera[0], 0);
    glVertex2i(-gl.camera[0], 100);
    glVertex2i(gl.xres + -gl.camera[0], 100);
    glVertex2i(gl.xres + -gl.camera[0], 0);
    glEnd();
}

ALuint alBuffer[11];
ALuint alSource[11];
ALint statel;

void setupAudio()
{
#ifdef ENABLE_AUDIO
    printf("AUDIO ENABLED\n");
    alutInit(0, NULL);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: alutInit()\n");
    }
    alGetError();

    float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);
    
    //click up and down
    alBuffer[0] = alutCreateBufferFromFile("./sound/select.wav"); 
    //select
    alBuffer[1] = alutCreateBufferFromFile("./sound/click.wav"); 
    //blast lightning
    alBuffer[2] = alutCreateBufferFromFile("./sound/Blast.wav"); 
    //Game menu
    alBuffer[3] = alutCreateBufferFromFile("./sound/MainMenu.wav"); 
    // Background Music
    alBuffer[4] = alutCreateBufferFromFile("./sound/theme1b.wav"); 
    //game over
    alBuffer[5] = alutCreateBufferFromFile("./sound/gameover.wav"); 
    //girl die scream
    alBuffer[6] = alutCreateBufferFromFile("./sound/scream.wav"); 
    //girl jumps
    alBuffer[7] = alutCreateBufferFromFile("./sound/jump.wav");
    //grabs object
    alBuffer[8] = alutCreateBufferFromFile("./sound/grab.wav"); 
    //grabs object
    alBuffer[9] = alutCreateBufferFromFile("./sound/kitty.wav"); 
    //ghost
    alBuffer[10] = alutCreateBufferFromFile("./sound/ghostdeath.wav"); 

    alGenSources(11, alSource);

    alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
    alSourcef(alSource[0], AL_GAIN, 1.0f);
    alSourcef(alSource[0], AL_PITCH, 1.0f);
    alSourcef(alSource[0], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);
    alSourcef(alSource[1], AL_GAIN, 1.0f);
    alSourcef(alSource[1], AL_PITCH, 1.0f);
    alSourcef(alSource[1], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[2], AL_BUFFER, alBuffer[2]);
    alSourcef(alSource[2], AL_GAIN, 0.5f);
    alSourcef(alSource[2], AL_PITCH, 1.0f);
    alSourcef(alSource[2], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[3], AL_BUFFER, alBuffer[3]);
    alSourcef(alSource[3], AL_GAIN, 1.0f);
    alSourcef(alSource[3], AL_PITCH, 1.1f);
    alSourcef(alSource[3], AL_LOOPING, AL_TRUE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[4], AL_BUFFER, alBuffer[4]);
    alSourcef(alSource[4], AL_GAIN, 0.3f);
    alSourcef(alSource[4], AL_PITCH, 1.0f);
    alSourcef(alSource[4], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[5], AL_BUFFER, alBuffer[5]);
    alSourcef(alSource[5], AL_GAIN, 1.0f);
    alSourcef(alSource[5], AL_PITCH, 1.0f);
    alSourcef(alSource[5], AL_LOOPING, AL_TRUE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[6], AL_BUFFER, alBuffer[6]);
    alSourcef(alSource[6], AL_GAIN, 1.0f);
    alSourcef(alSource[6], AL_PITCH, 1.0f);
    alSourcef(alSource[6], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[7], AL_BUFFER, alBuffer[7]);
    alSourcef(alSource[7], AL_GAIN, 1.0f);
    alSourcef(alSource[7], AL_PITCH, 1.0f);
    alSourcef(alSource[7], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[8], AL_BUFFER, alBuffer[8]);
    alSourcef(alSource[8], AL_GAIN, 1.0f);
    alSourcef(alSource[8], AL_PITCH, 1.0f);
    alSourcef(alSource[8], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[9], AL_BUFFER, alBuffer[9]);
    alSourcef(alSource[9], AL_GAIN, 0.15f);
    alSourcef(alSource[9], AL_PITCH, 1.0f);
    alSourcef(alSource[9], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
    alSourcei(alSource[10], AL_BUFFER, alBuffer[10]);
    alSourcef(alSource[10], AL_GAIN, 1.0f);
    alSourcef(alSource[10], AL_PITCH, 1.0f);
    alSourcef(alSource[10], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Audio setup error\n");
    }
#endif
}
//cleanup audio
void cleanupAudio()
{
#ifdef ENABLE_AUDIO
    alDeleteSources(1, &alSource[0]);
    alDeleteSources(1, &alSource[1]);
    alDeleteSources(1, &alSource[2]);
    alDeleteSources(1, &alSource[3]);
    alDeleteSources(1, &alSource[4]);
    alDeleteSources(1, &alSource[5]);
    alDeleteSources(1, &alSource[6]);
    alDeleteSources(1, &alSource[7]);
    alDeleteSources(1, &alSource[8]);
    alDeleteSources(1, &alSource[9]);
    alDeleteSources(1, &alSource[10]);


    alDeleteBuffers(1, &alBuffer[0]);
    alDeleteBuffers(1, &alBuffer[1]);
    alDeleteBuffers(1, &alBuffer[2]);
    alDeleteBuffers(1, &alBuffer[3]);
    alDeleteBuffers(1, &alBuffer[4]);
    alDeleteBuffers(1, &alBuffer[5]);
    alDeleteSources(1, &alSource[6]);
    alDeleteSources(1, &alSource[7]);
    alDeleteSources(1, &alSource[8]);
    alDeleteSources(1, &alSource[9]);
    alDeleteSources(1, &alSource[10]);

    ALCcontext *Context = alcGetCurrentContext();
    ALCdevice *Device = alcGetContextsDevice(Context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    alcCloseDevice(Device);
#endif
}

void playClick()
{
#ifdef ENABLE_AUDIO
    alSourcePlay(alSource[0]);
#endif
}

void playSelection()
{
#ifdef ENABLE_AUDIO    
    alSourcePlay(alSource[1]);
#endif
}

void playPoint()
{
#ifdef ENABLE_AUDIO
   // alGetSourcei(alSource[2], AL_SOURCE_STATE, &statel);
  // if (statel != AL_PLAYING) {
        alSourcePlay(alSource[2]);
  //  }
#endif
}

void playScream()
{
#ifdef ENABLE_AUDIO
    alSourcePlay(alSource[6]);
#endif
}

void playJump()
{
#ifdef ENABLE_AUDIO
    alSourcePlay(alSource[7]);
#endif
}

void playGrab()
{
#ifdef ENABLE_AUDIO
    alGetSourcei(alSource[8], AL_SOURCE_STATE, &statel);
    if (statel != AL_PLAYING) {
        alSourcePlay(alSource[8]);
    }
#endif
}

void playMeow()
{
#ifdef ENABLE_AUDIO
    alSourcePlay(alSource[9]);
#endif
}

void playGhostDeath()
{
#ifdef ENABLE_AUDIO
   //alGetSourcei(alSource[10], AL_SOURCE_STATE, &statel);
   //if (statel != AL_PLAYING) {
        alSourcePlay(alSource[10]);
   //}
#endif
}

void physicsAudio()
{
#ifdef ENABLE_AUDIO
    alGetSourcei(alSource[3], AL_SOURCE_STATE, &statel);
    if (gl.state == STATE_GAMEPAUSE || gl.state == STATE_STARTUP ||
            gl.state == STATE_CREDITS || gl.state == STATE_HIGHSCORE) {
        if (statel != AL_PLAYING) {
            alSourcePlay(alSource[3]);
        }
    } else if (statel == AL_PLAYING) {
        alSourceStop(alSource[3]);
    }

    alGetSourcei(alSource[4], AL_SOURCE_STATE, &statel);
    if (gl.state == STATE_GAMEPLAY) {
        if (statel != AL_PLAYING) {
            alSourcePlay(alSource[4]);
        }
    } else if (statel == AL_PLAYING) {
        alSourceStop(alSource[4]);
    }

    alGetSourcei(alSource[5], AL_SOURCE_STATE, &statel);
    if (gl.state == STATE_GAMEOVER) {
        if (statel != AL_PLAYING) {
            alSourcePlay(alSource[5]);
            alSourcePlay(alSource[6]);
        }
    } else if (statel == AL_PLAYING) {
        alSourceStop(alSource[5]);
        alSourceStop(alSource[6]);
    }
#endif
}

int create_tcp_socket();
char *get_ip(char *host);
char *build_get_query(char *host, const char *page);
//char was made into a constant to fix the error of deprecated conversation


std::string submit(std::string username, int score)
{
    struct sockaddr_in *remote;
    int sock;
    int tmpres;
    char *ip;
    char *get;
    char buf[BUFSIZ + 1];
    char host[] = "sleipnir.cs.csubak.edu";
    char page[] = "/~ksalinas/3350/scores.php?username=%s&score=%d";
    sock = create_tcp_socket();
    ip = get_ip(host);
    fprintf(stderr, "IP is %s\n", ip);
    remote = (struct sockaddr_in *) malloc(sizeof (struct sockaddr_in *));
    remote->sin_family = AF_INET;
    tmpres = inet_pton(AF_INET, ip, (void *) (&(remote->sin_addr.s_addr)));
    if (tmpres < 0) {
        perror("Can't set remote->sin_addr.s_addr");
        exit(1);
    } else if (tmpres == 0) {
        fprintf(stderr, "%s is not a valid IP address\n", ip);
        exit(1);
    }
    remote->sin_port = htons(PORT);
    if (connect(sock, (struct sockaddr *) remote, 
            sizeof (struct sockaddr)) < 0) {
        perror("Could not connect");
        exit(1);
    }
    char pageString[512];
    sprintf(pageString, page, username.c_str(), score);
    get = build_get_query(host, pageString);
    fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);
    //Send the query to the server
    unsigned int sent = 0; //unsigned int to solve the error
    while (sent < strlen(get)) {
        tmpres = send(sock, get + sent, strlen(get) - sent, 0);
        if (tmpres == -1) {
            perror("send command, Can't send query");
            exit(1);
        }
        sent += tmpres;
    }
    //now it is time to receive the page
    memset(buf, 0, sizeof (buf));
    int htmlstart = 0;
    char * htmlcontent;
    std::string result;
    while ((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0) {
        if (htmlstart == 0) {
            /* Under certain conditions this will not work.
             * If the \r\n\r\n part is splitted into two messages
             * it will fail to detect the beginning of HTML content
             */
            htmlcontent = strstr(buf, "\r\n\r\n");
            if (htmlcontent != NULL) {
                htmlstart = 1;
                htmlcontent += 4;
            }
        } else {
            htmlcontent = buf;
        }
        if (htmlstart) {
            result = htmlcontent;
            //include the "%s" to solve the issue of making it 
            //a string literal.
        }
        //memset(buf, 0, tmpres);
    }
    if (tmpres < 0) {
        perror("Error receiving data");
    }
    free(get);
    free(remote);
    free(ip);
    close(sock);
    return result;
}

void pause(int e)
{

    int temp = time(NULL) + e;

    while (temp > time(NULL));

}

int create_tcp_socket()
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("Can't create TCP socket");
        exit(1);
    }
    return sock;
}
//change to const char

char *get_ip(char *host)
{
    struct hostent *hent;
    //ip address format  123.123.123.123
    int iplen = 15;
    char *ip = (char *) malloc(iplen + 1);
    memset(ip, 0, iplen + 1);
    if ((hent = gethostbyname(host)) == NULL) {
        herror("Can't get IP host by name");
        exit(1);
    }
    //for the bottom, we add 1 onto iplen so that we can fix the 
    //failed hostname
    if (inet_ntop(AF_INET, (void *) hent->h_addr_list[0],
        ip, iplen + 1) == NULL)
 {
        perror("Can't resolve host with inet_ntop");
        exit(1);
    }
    return ip;
}
//change char into a constant

char *build_get_query(char *host, const char *page)
{
    char *query;
    const char *getpage = page;
    //added a constant to char to solve problem of deprecated conversion 
    //from string constant to char the following char *tpl was made into a 
    //constant to fix the error to pass string literals.
    const char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
    if (getpage[0] == '/') {
        getpage = getpage + 1;
        fprintf(stderr, "Removing leading \"/\", converting %s to %s\n", 
                page, getpage);
    }
    // -5 is to consider the %s %s %s in tpl and the ending \0
    query = (char *) malloc(strlen(host) + strlen(getpage) + strlen(USERAGENT) 
            + strlen(tpl) - 5);
    sprintf(query, tpl, getpage, host, USERAGENT);
    return query;
}

void updateScores(std::string username, int score)
{
    //delete all scores
    gl.scores.clear();
    
    //Send current and fetch new high scores
    std::string text = submit(username, score);
    printf("Scores are: %s\n", text.c_str());
    
    std::stringstream ss(text);
    std::string u;
    int s;
    while(ss >> u >> s) {
        gl.scores.push_back(Score(u,s));
    }
}

void renderUsernameInput()
{
    glPushMatrix();
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glVertex2i(gl.xres/2-150, gl.yres/2-250);
    glVertex2i(gl.xres/2-150, gl.yres/2-200);
    glVertex2i(gl.xres/2+150, gl.yres/2-200);
    glVertex2i(gl.xres/2+150, gl.yres/2-250);
    glEnd();
    
    Rect r;
    r.center = 1;
    r.left = gl.xres/2;
    r.bot = gl.yres/2-245;
    ggprint16(&r, 0, 0xffffff, gl.username.c_str());
   
    r.center = 0;
    r.bot = gl.yres/2-200;
    r.left = gl.xres/2-150;
    ggprint16(&r, 0, 0xff00ff, "Enter username and press ENTER to submit!");
    
    glPopMatrix();
}

void checkUserNameInput(int key) 
{
    if(gl.state == STATE_GAMEOVER) {
        switch(key) {
        case XK_Return:
            if(!gl.username.empty()) {
                updateScores(gl.username, gl.savescore);
            }
            gl.state = STATE_HIGHSCORE;
            break;
        case XK_Escape:
            gl.username = "";
            break;
        case XK_BackSpace:
            if(!gl.username.empty()) {
                gl.username = gl.username.substr(0, gl.username.size()-1);
            }
            break;
        default:
            if(key >= 97 && key <= 122) {
                gl.username += (char)key;
            }
        }
    }
}
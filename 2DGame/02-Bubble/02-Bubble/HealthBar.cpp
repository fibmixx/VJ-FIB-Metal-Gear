#include "HealthBar.h"

void HealthBar::init(int screenWidth, int screenHeight,ShaderProgram& shaderProgram) {
    SCREEN_WIDTH = screenWidth;
    SCREEN_HEIGHT = screenHeight;

    shaderProg = &shaderProgram;
    loadTextures();
}

void HealthBar::loadTextures() {
    healthTexture.loadFromFile("images/health.png", TEXTURE_PIXEL_FORMAT_RGBA);
    weaponTexture.loadFromFile("images/weapon.png", TEXTURE_PIXEL_FORMAT_RGBA);
    keyTexture.loadFromFile("images/card.png", TEXTURE_PIXEL_FORMAT_RGBA);

    lifeLabelTexture.loadFromFile("images/life.png", TEXTURE_PIXEL_FORMAT_RGBA);
    
}

void HealthBar::renderTexture(Texture& texture, int x, int y, int size) {
    glEnable(GL_TEXTURE_2D);
    texture.use();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + size, y);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + size, y + size);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + size);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
//mostrar barra de vida
void HealthBar::render(int currentHealth, int maxHealth) {  
   
    glPushAttrib(GL_ALL_ATTRIB_BITS);

  
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

 
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    float healthPercent = (float)currentHealth / (float)maxHealth;

   
    int barWidth = 200;
    int barHeight = 25;
    int x = 200;
    int y = SCREEN_HEIGHT - 60;

    int lifeLabelWidth = 91;   
    int lifeLabelHeight = 24;  
    int lifeLabelX = x - lifeLabelWidth - 15; 
    int lifeLabelY = y - (lifeLabelHeight - barHeight) / 2;

    
    renderTextureWithSize(lifeLabelTexture, lifeLabelX, lifeLabelY, lifeLabelWidth, lifeLabelHeight);
    
    //vida actual
    if (currentHealth > 0) {
        int currentWidth = (int)(barWidth * healthPercent);

        if (healthPercent > 0.5f) {
            glColor3f(0.0f, 0.8f, 0.1f);
        }
        else if (healthPercent > 0.2f) {
            glColor3f(0.9f, 0.7f, 0.0f);
        }
        else {
            glColor3f(0.8f, 0.1f, 0.1f);
        }

        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + currentWidth, y);
        glVertex2f(x + currentWidth, y + barHeight);
        glVertex2f(x, y + barHeight);
        glEnd();
    }

    // marc blanc
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + barWidth, y);
    glVertex2f(x + barWidth, y + barHeight);
    glVertex2f(x, y + barHeight);
    glEnd();

 
    glUseProgram(currentProgram);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib(); 
}

void HealthBar::renderTextureWithSize(Texture& texture, int x, int y, int width, int height) {
    glEnable(GL_TEXTURE_2D);
    texture.use();

    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);                   
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y);          
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height);  
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height);          
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//mostrar objecte
void HealthBar::renderObj(const std::vector<int>& inventory, int currentEquipped) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    glUseProgram(0);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   
    int objDisplayX = 500;
    int objDisplayY = SCREEN_HEIGHT - 80; 
    int objSize = 60;
    int spacing = 10;

    if (!inventory.empty()) {
        glColor4f(0.1f, 0.1f, 0.1f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(objDisplayX - 5, objDisplayY - 5);
        glVertex2f(objDisplayX + (objSize + spacing) * inventory.size() + 5, objDisplayY - 5);
        glVertex2f(objDisplayX + (objSize + spacing) * inventory.size() + 5, objDisplayY + objSize + 5);
        glVertex2f(objDisplayX - 5, objDisplayY + objSize + 5);
        glEnd();
    }

    for (int i = 0; i < inventory.size(); ++i) {
        int itemX = objDisplayX + i * (objSize + spacing);
        int itemY = objDisplayY;

 
        if (i == currentEquipped) {
            glColor3f(1.0f, 0.8f, 0.0f);
            glLineWidth(3.0f);
        }
        else {
            glColor3f(0.5f, 0.5f, 0.5f);
            glLineWidth(1.5f);
        }

        glBegin(GL_LINE_LOOP);
        glVertex2f(itemX, itemY);
        glVertex2f(itemX + objSize, itemY);
        glVertex2f(itemX + objSize, itemY + objSize);
        glVertex2f(itemX, itemY + objSize);
        glEnd();

        switch (inventory[i]) {
        case 0: // HEALTH
            renderTexture(healthTexture, itemX + 2, itemY + 2, objSize - 4);
            break;
        case 1: // WEAPON
            renderTexture(weaponTexture, itemX + 2, itemY + 2, objSize - 4);
            break;
        case 2: // KEY
            renderTexture(keyTexture, itemX + 2, itemY + 2, objSize - 4);
            break;
       
        }

        // Número del ítem (posición en el inventario)
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(itemX + 5, itemY + 12);
        char numStr[3];
        sprintf(numStr, "%d", i + 1); 
    }

    // Restaurar estat OpenGL
    glUseProgram(currentProgram);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}
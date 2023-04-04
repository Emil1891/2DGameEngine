#include "SpriteAnimated.h"

namespace engine {
    SpriteAnimated* SpriteAnimated::getInstance(int x, int y, std::initializer_list<std::string> imageFileNames, const bool playOnce)
    {
        return new SpriteAnimated(x, y, imageFileNames, playOnce);
    }

    void SpriteAnimated::engineUpdate(std::vector<GameObject*> objs) 
    {
        Sprite::engineUpdate(objs); 
        if (paused) {
            startTime = SDL_GetTicks(); 
            return; 
        }

        if (SDL_GetTicks() - startTime < msBetweenAnimUpdate / animationSpeed) // Tillräcklig tid har inte passerat för att updatera bilden
            return; 

        changeImage(images[currentImageIndex++], !updateSizeOnImageUpdate); 
        
        if (currentImageIndex >= images.size()) {
            if (playOnce)
                currentScene->remove(this); 
            currentImageIndex = 0;
        }
        startTime = SDL_GetTicks(); // "Återställ" timer 
    }

    void SpriteAnimated::pauseAnimation()
    {
        if (paused)
            return;
        paused = true; 
        changeImage(images[0]); 
        currentImageIndex = 0; 
    }

    void SpriteAnimated::changeAnimation(std::initializer_list<std::string> imageFileNames)
    {
        images = imageFileNames; 
        currentImageIndex = 0; 
    }

    SpriteAnimated::SpriteAnimated(int x, int y, std::initializer_list<std::string> imageFiles, const bool playOnce) : Sprite(x, y, *imageFiles.begin()), images(imageFiles), playOnce(playOnce)
    {
    }
}
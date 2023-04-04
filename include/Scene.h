#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

namespace engine {

    class GameObject;

    class Scene // Klasser som �verskuggar b�r vara singletons 
    {
    public:
        
        /// <summary> Kallas direkt scenen laddas in. H�r b�r alla objekt (som �r med fr�n b�rjan) skapas och l�ggas in med add() </summary>
        virtual void enter() = 0; 

        /// <summary> Kallas precis innan scenen byter till n�sta. H�r �r det meningen att st�da undan alla skapade objekt (fungerar allts� som klassens destruktor). 
        /// Om den inte �verskuggas tas alla objekt i gameObjects bort automatiskt och rensar vektorerna (�terst�ller scenen, exkl. ev. globala variablar). 
        /// Det �r allts� s�llan som den beh�ver eller b�r �verskuggas. </summary>
        virtual void exit();
 
        void handleEvent(SDL_Event& e); 

        // L�gger till alla objekt som ska ing� i scenen 
        void add(GameObject*);
        // L�gger till alla objekt som ska ing� i scenen i form av en lista: add({...})
        void add(std::initializer_list<GameObject*>);

        /// <summary> Tar bort objektet och f�rst�r det (delete). Rekommenderat att s�tta pekaren till 0 efter denna funktion kallats. </summary>
        /// <param name=""> Objektet som ska tas bort/f�rst�ras. </param>
        void remove(GameObject* obj);

        /// <summary> Hittar ett specifikt objekt med visst namn. </summary>
        /// <param name="name"> Namnet p� objektet som letas efter, s�tts med setName(). </param>
        /// <returns> Returnerar pekare till det f�rsta objekt med det namn som letades efter eller nullptr om det inte hittades. </returns>
        GameObject* find(const std::string name) const;

        /// <summary> Hittar alla objekt med visst namn. </summary>
        /// <param name="name"> Namnet p� objekten som letas efter, s�tts med setName(). </param>
        /// <returns> Returnerar alla objekt som hittades med namnet som anges i en vector. Tom vector om inget hittas. </returns>
        std::vector<GameObject*> findAll(const std::string name) const;

        void update(); 
        void render();

        void updateAddedRemovedObjects(); 

        /// <summary> Laddar en ny scen. </summary>
        /// <param name="nextScene"> Scenen som ska laddas in. </param>
        void loadScene(Scene* scene) { nextScene = scene; }

        /// <summary> Laddar in den scen som angivits med loadScene(). Har ingen scen satts h�nder inget. Kallas p� fr�n spelloopen automatiskt varje frame. </summary>
        /// <returns> Returnerar den nya scenen (eller nuvarande om det inte byts) </returns> 
        Scene* changeScene();

        virtual ~Scene() {}

    private:
        std::vector<GameObject*> gameObjects, added, removed; // De objekt som tillh�r denna scen 
        Scene* nextScene = nullptr;
    };

    extern Scene* currentScene;
}

#endif
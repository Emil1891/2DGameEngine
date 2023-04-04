#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

namespace engine {

    class GameObject;

    class Scene // Klasser som överskuggar bör vara singletons 
    {
    public:
        
        /// <summary> Kallas direkt scenen laddas in. Här bör alla objekt (som är med från början) skapas och läggas in med add() </summary>
        virtual void enter() = 0; 

        /// <summary> Kallas precis innan scenen byter till nästa. Här är det meningen att städa undan alla skapade objekt (fungerar alltså som klassens destruktor). 
        /// Om den inte överskuggas tas alla objekt i gameObjects bort automatiskt och rensar vektorerna (återställer scenen, exkl. ev. globala variablar). 
        /// Det är alltså sällan som den behöver eller bör överskuggas. </summary>
        virtual void exit();
 
        void handleEvent(SDL_Event& e); 

        // Lägger till alla objekt som ska ingå i scenen 
        void add(GameObject*);
        // Lägger till alla objekt som ska ingå i scenen i form av en lista: add({...})
        void add(std::initializer_list<GameObject*>);

        /// <summary> Tar bort objektet och förstör det (delete). Rekommenderat att sätta pekaren till 0 efter denna funktion kallats. </summary>
        /// <param name=""> Objektet som ska tas bort/förstöras. </param>
        void remove(GameObject* obj);

        /// <summary> Hittar ett specifikt objekt med visst namn. </summary>
        /// <param name="name"> Namnet på objektet som letas efter, sätts med setName(). </param>
        /// <returns> Returnerar pekare till det första objekt med det namn som letades efter eller nullptr om det inte hittades. </returns>
        GameObject* find(const std::string name) const;

        /// <summary> Hittar alla objekt med visst namn. </summary>
        /// <param name="name"> Namnet på objekten som letas efter, sätts med setName(). </param>
        /// <returns> Returnerar alla objekt som hittades med namnet som anges i en vector. Tom vector om inget hittas. </returns>
        std::vector<GameObject*> findAll(const std::string name) const;

        void update(); 
        void render();

        void updateAddedRemovedObjects(); 

        /// <summary> Laddar en ny scen. </summary>
        /// <param name="nextScene"> Scenen som ska laddas in. </param>
        void loadScene(Scene* scene) { nextScene = scene; }

        /// <summary> Laddar in den scen som angivits med loadScene(). Har ingen scen satts händer inget. Kallas på från spelloopen automatiskt varje frame. </summary>
        /// <returns> Returnerar den nya scenen (eller nuvarande om det inte byts) </returns> 
        Scene* changeScene();

        virtual ~Scene() {}

    private:
        std::vector<GameObject*> gameObjects, added, removed; // De objekt som tillhör denna scen 
        Scene* nextScene = nullptr;
    };

    extern Scene* currentScene;
}

#endif
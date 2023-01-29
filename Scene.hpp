/**
 * TODO: Decide how to ogranize scene, game objects, drawable objects and renderer
 * 
 * See answers in this post:
 * https://gamedev.stackexchange.com/questions/153879/scene-components-and-renderer
 * "
 *      In the Scene itself, store the array of Renderable objects for the scene separately from the nodes in the scene. 
 *      You can have nodes in the scene fill this list up with Renderable objects they'd like to have drawn 
 *      as part of the routine processing of your logic update. Then, at the start of rendering for that frame, 
 *      you just give the renderer the list of Renderable objects that the scene has built up for that frame, 
 *      and let it do its job.
 *      
 *      The key idea here is that the "scene" is a high level concept and the renderer is a lower-level one. 
 *      High level APIs can know about, and use, lower-level ones but such visibility should not go the other way. 
 *      By enforcing that kind of unidirectional data flow you can keep code less coupled and simpler.
 * "
 * 
 * ==> "GameObjects" use the Scene to "subscribe" to a "Renderable" and the scene dispatches all Renderables to the 
 *      Renderer. The scene probably needs to subscribe Renderables at the Renderer too in order to keep OpenGL-Stuff
 *      like vbos etc. separated from the Scene 
 * 
*/

#include "Transform.hpp"

#include <vector>
#include <map>

class GameObject;   // holds parent and children
class Drawable;     // holds vertex data
class Renderer;

class Scene {

    void draw();  // send a sorted list of drawables to the renderer
    
    std::vector<GameObject> objects; 
    std::vector<Drawable> drawables;

    // every GameObject with a drawable registers its drawable index here
    // TODO: Working with indices might be cumbersome
    // TODO: We also need a good way to sort the drawables (for transparency, screen clipping, grouping Instanced Draws, ...)
    // ==> This probably will be its own class 
    std::map<int, int> registeredDrawables;   

    // More than one? Or has the renderer sub-renderer? (ie. a debugging renderer)
    Renderer* renderer;
};
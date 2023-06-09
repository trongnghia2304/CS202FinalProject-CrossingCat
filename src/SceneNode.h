#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>
#include <set>

/**
 * @brief A node in the scene graph.
 */
class SceneNode : public sf::Drawable,
                  public sf::Transformable,
                  private sf::NonCopyable {
public:
    typedef std::unique_ptr<SceneNode> Ptr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

    SceneNode();

    /**
     * Add new child to the node
     *
     * @param child: the child to be added
     */
    void attachChild(Ptr child);

    /**
     * Remove a child from the node
     *
     * @param child: reference to the to-be-deleted child
     *
     * @return a Ptr to child
     */
    Ptr detachChild(const SceneNode &child);

    /**
     * Update the node and all the children node
     *
     * @param dt: time elapsed since last frame
     */
    void update(sf::Time dt);

    /**
     * @return the absolute transform (all transform of node hierarchy combined)
     */
    sf::Transform getAbsTransform() const;

    /**
     * @return the absolute position (coordinate in the windows)
     */
    sf::Vector2f getAbsPosition() const;

    /**
     * Check collision between 2 SceneNode. Derived class from SceneNode should
     * implement `getBoundingRect` to use this function
     */
    friend bool checkCollision(const SceneNode& lnode, const SceneNode& rnode);

    /**
     * Check collision of the `node` with `this` and all children of `this`
     *
     * @param node Node to check collision
     * @param collisions Variables to store collision pair
     */
    void checkNodeCollision(SceneNode& node, std::set<Pair>& collisions);

    /**
     */
    void checkSceneCollision(SceneNode& node, std::set<Pair>& collisions);

    /**
     * Set the debug mode of the node
     * @param on: on/off
     * @param recursive: apply to all children
     */
    void setDebug(bool on, bool recursive = false);

private:
    void drawBoundingBox(sf::RenderTarget& target, sf::RenderStates states) const;
    bool boundingBoxVisible = false;
    void draw(sf::RenderTarget &target, sf::RenderStates state) const final;

    /**
     * Derived class should implement this method to draw itself
     *
     * @param target: Target to draw to 
     * @param state: State stored all the applied state forwarded from the
     * node's parent 
     */
    virtual void drawCurrent(sf::RenderTarget &target,
                             sf::RenderStates state) const;

    /**
     * Derived class should implement this method to update itself
     */
    virtual void updateCurrent(sf::Time dt);

    /**
     * Call the update methods of children
     */
    void updateChildren(sf::Time dt);

protected:
    /**
     * Implement this to allow collision detection
     */
    virtual sf::FloatRect getBoundingRect() const;

private:
    std::vector<Ptr> mChildren;
    SceneNode *mParent;
};


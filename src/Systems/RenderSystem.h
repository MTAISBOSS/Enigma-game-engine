#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <algorithm>
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "SDL_rect.h"
#include "SDL.h"

class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore) {

        struct RenderableEntity{
            TransformComponent transformComponent;
            SpriteComponent spriteComponent;
        };
        std::vector<RenderableEntity> renderableEntities;

        for(auto entity : GetSystemEntities()){
            RenderableEntity renderableEntity;
            renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
            renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
            renderableEntities.push_back(renderableEntity);
        }
        std::sort(renderableEntities.begin(),renderableEntities.end(),[](const RenderableEntity& a,const RenderableEntity& b){
            return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
        });
        for (auto entity: renderableEntities) {
            const auto sprite = entity.spriteComponent;
            const auto transform = entity.transformComponent;

            SDL_Rect srcRect = sprite.srcRect;
            SDL_Rect dstRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)
            };
            SDL_RenderCopyEx(
                    renderer,
                    assetStore->GetTexture(sprite.assetId),
                    &srcRect,
                    &dstRect,
                    transform.rotation,
                    nullptr,
                    SDL_FLIP_NONE);

        }
    }
};

#endif //RENDERSYSTEM_H

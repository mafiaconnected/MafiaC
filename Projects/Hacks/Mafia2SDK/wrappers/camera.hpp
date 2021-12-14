#pragma once

namespace M2
{
    namespace Wrappers
    {
        static const float DIRECTION_MULTIPLIER = 8192.0f;

        /**
         * Returns an aproximate position of the place local player is looking at
         */
        static inline Vector3 GetLookAt()
        {
            Vector3 lookAt;
            auto camera = M2::C_GameCamera::Get()->GetCamera(1);
            if (!camera)
                return lookAt;

            auto vCamPos = camera->m_vecCamPos;
            auto vCamUp = camera->m_vecCamUp;
            auto vDir = (vCamUp - vCamPos);

            lookAt = (vCamPos + vDir * DIRECTION_MULTIPLIER);
            return lookAt;
        }
    };
};


#ifdef MAFIA_SDK_IMPLEMENTATION

namespace M2 {
    void *GetCameraWorldViewProjection()
    {
        auto camera = M2::C_GameCamera::Get()->GetCamera(1);
        return camera ? (void *)&camera->m_pGameCamera->m_worldViewProjection : NULL;
    }
}

#endif // MAFIA_SDK_IMPLEMENTATION


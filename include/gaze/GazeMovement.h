//
// Created by geislerd on 22.03.17.
//

#ifndef GAZEMOVEMENT_H
#define GAZEMOVEMENT_H

#include <gaze/Gaze.h>

namespace saliency_sandbox {
    namespace gaze {
        class GazeMovement {
        public:
            enum Type {
                FIXATION = 0,
                SACCADE = 1,
                UNKNOWN = -1
            };
        private:
            GazeMovement* m_prev;
            Gaze m_gaze;
            Type m_type;
        public:

            enum Component {
                GAZE = 0,
                TYPE = 1,
                PREV = 2,
            };

            template<Component _component>
            class Selector {};

            GazeMovement(GazeMovement* prev, const Gaze& gaze, const Type& type);
            GazeMovement(GazeMovement* prev, const Gaze& gaze);
            GazeMovement(const Gaze& gaze);
            GazeMovement* prev();
            void prev(GazeMovement* prev);
            Gaze& gaze();
            void gaze(const Gaze& gaze);
            Type& type();
            void type(const Type& type);
            GazeMovement next(const Gaze& gaze);
            GazeMovement next(const Gaze& gaze, const Type& type);
        };

        template<>
        class GazeMovement::Selector<GazeMovement::GAZE> : public saliency_sandbox::core::Node::
                               template Input<GazeMovement>::
                               template Output<Gaze> {
            Selector();
            void calc() override;
            void reset() override;
        };

        template<>
        class GazeMovement::Selector<GazeMovement::TYPE> : public saliency_sandbox::core::Node::
                               template Input<GazeMovement>::
                               template Output<GazeMovement::Type> {
            Selector();
            void calc() override;
            void reset() override;
        };

        template<>
        class GazeMovement::Selector<GazeMovement::PREV> : public saliency_sandbox::core::Node::
                               template Input<GazeMovement>::
                               template Output<GazeMovement> {
            Selector();
            void calc() override;
            void reset() override;
        };
    }
}
#endif //GAZEMOVEMENT_H

//
// Created by geislerd on 22.03.17.
//

#include <gaze/GazeMovement.h>

namespace saliency_sandbox {
    namespace gaze {
        GazeMovement::GazeMovement(GazeMovement* prev, const Gaze& gaze, const Type& type) : m_prev(prev), m_gaze(gaze), m_type(type) {

        }
        GazeMovement::GazeMovement(GazeMovement* prev, const Gaze& gaze) : m_prev(prev), m_gaze(gaze), m_type(UNKNOWN) {

        }
        GazeMovement::GazeMovement(const Gaze& gaze) : m_prev(nullptr), m_gaze(gaze), m_type(UNKNOWN) {

        }

        GazeMovement* GazeMovement::prev() {
            return this->m_prev;
        }

        void GazeMovement::prev(GazeMovement* prev) {
            this->m_prev = prev;
        }

        Gaze& GazeMovement::gaze() {
            return this->m_gaze;
        }

        void GazeMovement::gaze(const Gaze& gaze) {
            this->m_gaze = gaze;
        }

        GazeMovement::Type& GazeMovement::type() {
            return this->m_type;
        }

        void GazeMovement::type(const Type& type) {
            this->m_type = type;
        }

        GazeMovement GazeMovement::next(const Gaze& gaze) {
            return next(gaze,UNKNOWN);
        }

        GazeMovement GazeMovement::next(const Gaze& gaze, const Type& type) {
            return GazeMovement(this,gaze,type);
        }

        GazeMovement::Selector<GazeMovement::GAZE>::Selector() {
            this->template input<0>()->name("gaze_movement");
            this->template output<0>()->name("gaze");
        }

        void GazeMovement::Selector<GazeMovement::GAZE>::calc() {
            this->template output<0>()->value(&(this->template input<0>()->value()->m_gaze));
        }

        void GazeMovement::Selector<GazeMovement::GAZE>::reset() {
        }

        GazeMovement::Selector<GazeMovement::TYPE>::Selector() {
            this->template input<0>()->name("gaze_movement");
            this->template output<0>()->name("type");
        }

        void GazeMovement::Selector<GazeMovement::TYPE>::calc() {
            this->template output<0>()->value(&(this->template input<0>()->value()->m_type));
        }

        void GazeMovement::Selector<GazeMovement::TYPE>::reset() {
        }

        GazeMovement::Selector<GazeMovement::PREV>::Selector() {
            this->template input<0>()->name("gaze_movement");
            this->template output<0>()->name("prev");
        }

        void GazeMovement::Selector<GazeMovement::PREV>::calc() {
            this->template output<0>()->value(this->template input<0>()->value()->m_prev);
        }

        void GazeMovement::Selector<GazeMovement::PREV>::reset() {
        }

        template class GazeMovement::Selector<GazeMovement::GAZE>;
        template class GazeMovement::Selector<GazeMovement::TYPE>;
        template class GazeMovement::Selector<GazeMovement::PREV>;
    }
}
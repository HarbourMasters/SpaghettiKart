#include <actors.h>

/**
 * @brief Updates the train engine actor.
 * Used in Kalimari Desert.
 * Update wheels and make sound, for his update position it's in vehicle.
 *
 * @param arg0
 */
void update_actor_train_engine(struct TrainCar* arg0) {
    arg0->wheelRot -= 0x666;

    if (arg0->unk_08 != 0.0f) {
        arg0->unk_08 = 0.0f;
        func_800C9D80(arg0->pos, arg0->velocity, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x00));
    }
}

/**
 * @brief Updates the train tender actor.
 * Used in Kalimari Desert.
 * Update wheels, for his update position it's in vehicle.
 * @param tender
 */
void update_actor_train_tender(struct TrainCar* tender) {
    tender->wheelRot -= 0x4FA;
}

/**
 * @brief Updates the train passenger car actor.
 * Used in Kalimari Desert.
 * Update wheels, for his update position it's in vehicle.
 *
 * @param arg0
 */
void update_actor_train_passenger_car(struct TrainCar* arg0) {
    arg0->wheelRot -= 0x666;
}

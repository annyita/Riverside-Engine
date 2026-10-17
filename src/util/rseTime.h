#ifndef RIVERSIDE_TIME_CLASS_H
#define RIVERSIDE_TIME_CLASS_H

namespace RSE {

	class rseTime {
	public:

		struct timer {
			float deltatime;
			float lastFrameTime;
			float time;
			float priorTime;
		};

		static timer mainTimer;

		static void updateTimer(timer& t);

	private:
	};
}

#endif
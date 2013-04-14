#include <linux/input.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define THRESHOLD 875
#define DEADZONE 125
#define POLLRATE 500

void enable(void);
void disable(void);
void set_delay(int);
void decode(struct input_event);
void cleanup(void);
void rotate(void);

int eventfd;

int rot = 0, ax = 0, ay = 0, az = 0;

int main(int argc, char **argv) {
	enable();
	set_delay(POLLRATE);

	eventfd = open("/dev/input/event3", O_RDONLY);
	if (eventfd < 0) {
		perror("open(/dev/input/event3) failed");
		exit(1);
	}
	atexit(cleanup);
	if (flock(eventfd, LOCK_EX))
		perror("flock(/dev/input/event3) failed");

	struct input_event ev;

	while (1) {
		read(eventfd, &ev, sizeof(struct input_event));
		decode(ev);
	}

	return 0;
}

void enable(void) {
	int fd;
	fd = open("/sys/bus/i2c/drivers/lsm303dlh_acc_sysfs/0-0018/enable_device", O_WRONLY);
	if (fd < 0) {
		perror("can't open accelerometer enable file");
		exit(1);
	}
	if (write(fd, "1", 2) != 2) {
		perror("can't enable accelerometer");
		close(fd);
		exit(1);
	}
	close(fd);
	return;
}
void disable(void) {
	int fd;
	fd = open("/sys/bus/i2c/drivers/lsm303dlh_acc_sysfs/0-0018/enable_device", O_WRONLY);
	if (fd < 0) {
		perror("can't open accelerometer enable file");
		exit(1);
	}
	if (write(fd, "0", 2) != 2) {
		perror("can't disable accelerometer");
		close(fd);
		exit(1);
	}
	close(fd);
	return;
}

void set_delay(int delay) {
	int fd, nchars;
	char s_delay[8];
	
	nchars = snprintf(s_delay, 8, "%d", delay);
	nchars = (nchars > 8) ? 8 : nchars;

	fd = open("/sys/bus/i2c/drivers/lsm303dlh_acc_sysfs/0-0018/pollrate_ms", O_WRONLY);
	if (fd < 0) {
		perror("can't open accelerometer poll rate file");
		exit(1);
	}
	if (write(fd, s_delay, nchars) != nchars) {
		perror("can't set delay on accelerometer");
		close(fd);
		exit(1);
	}
	close(fd);
	return;
}

void decode(struct input_event ev) {
	char oldrot = rot;
	if (ev.type == EV_ABS) {
		switch (ev.code) {
		case ABS_X:
			ax = ev.value;
		case ABS_Y:
			ay = ev.value;
		case ABS_Z:
			az = ev.value;
		}
	}
	if ((ev.type == 0) && (ev.code == 0)) {
		printf("raw: (%d, %d, %d)\n", ax, ay, az);
		if ((ay > THRESHOLD) && (abs(ax) < DEADZONE)) rot = 90; // portrait 270deg.
		else if ((ax > THRESHOLD) && (abs(ay) < DEADZONE)) rot = 0; // 0deg.
		else if ((ay < -THRESHOLD) && (abs(ax) < DEADZONE)) rot = ' '; // 90deg.
		else if ((ax < -THRESHOLD) && (abs(ay) < DEADZONE)) rot = ' '; // 180deg.
		printf("rotation: %d (?)\n", rot);
	}
	if (oldrot != rot) rotate();
}
void cleanup(void) {
	disable();
	close(eventfd);
}
void rotate(void) {
	char rcmd[12];
	snprintf(rcmd, 12, "tsrotate %d", rot);
	system(rcmd);
}

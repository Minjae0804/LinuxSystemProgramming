
class FDGuard {
	int fd;
public:
	FDGuard(int fd) : fd(fd) {}
	~FDGuard() { if (fd != -1) close(fd); }
	int get() const { return fd; }

	FDGuard(const FDGuard&) = delete;
	FDGuard& operator=(const FDGuard&) = delete;
};
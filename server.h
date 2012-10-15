enum Difficulty {
  EASY = 0,
  MEDIUM,
  HARD
};

class Server {
public:
  Server(int p) {port = p;}

  void server_loop() throw(std::out_of_range);

private:
  int port;
  bool display;

  void set_difficulty(Difficulty){}
  void toggle_display();
  void help();
};


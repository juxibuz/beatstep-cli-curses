#include <iostream>
#include <cstdlib>
#include "BeatStep.hpp"

#include <ncurses.h>
#include <string>

#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"

int main(int argc, char *argv[]) {
  CLI::App app{"Use sysex to control BeatStep"};
  app.require_subcommand();

  int device = 1;
  std::string filename;

  auto subList = app.add_subcommand("list", "List available MIDI devices");
  auto subCurses = app.add_subcommand("curses", "Launch curses interface");
  
  auto subLoad = app.add_subcommand("load", "Load a .beatstep preset file on device");
  subLoad->add_option("-d,--device", device, "The device to use (see list)");
  subLoad->add_option("FILE", filename, "The .beatstep file")->required();

  auto subSave = app.add_subcommand("save", "Save a .beatstep preset file from device");
  subSave->add_option("-d,--device", device, "The device to use (see list)");
  subSave->add_option("FILE", filename, "The .beatstep file")->required();

  // auto subUpdate = app.add_subcommand("update", "Install a .led firmware file on device");
  // subUpdate->add_option("-d,--device", device, "The device to use (see list)");
  // subUpdate->add_option("FILE", filename, "The .led file")->required();

  int led = 0;
  std::string color = "red";
  auto subColor = app.add_subcommand("color", "Change color of an LED");
  subColor->add_option("-d,--device", device, "The device to use (see list)");
  subColor->add_option("LED", led, "The pad-number to change color")->required();
  subColor->add_option("COLOR", color, "The color to change it to (off, red, pink, blue)")->required();

  auto subFw = app.add_subcommand("fw", "Get the firmware version on the device");
  subFw->add_option("-d,--device", device, "The device to use (see list)");

  int pp;
  int cc;
  int vv;
  bool intOut = false;

  auto subGet = app.add_subcommand("get", "Get a param-value");
  subGet->add_option("-d,--device", device, "The device to use (see list)");
  subGet->add_flag("-i,--int", intOut, "Output decimal value, instead of hex");
  subGet->add_option("PROGRAM", pp, "The number of the program")->required();
  subGet->add_option("CONTROL", cc, "The number of the control")->required();

  auto subSet = app.add_subcommand("set", "Set a param-value");
  subSet->add_option("-d,--device", device, "The device to use (see list)");
  subSet->add_option("PROGRAM", pp, "The number of the program")->required();
  subSet->add_option("CONTROL", cc, "The number of the control")->required();
  subSet->add_option("VALUE", vv, "The number of the value to set")->required();

  CLI11_PARSE(app, argc, argv);

  BeatStep* bs = new BeatStep();

  if (app.got_subcommand(subList)) {
    bs->list();
    return 0;
  }

  bool n = true;

  if (app.got_subcommand(subColor)) {
    bs->openPort(device - 1);
    BeatstepColor c = BEATSTEP_COLORS_OFF;
    if (color == "red") {
      c = BEATSTEP_COLORS_RED;
    }
    if (color == "pink") {
      c = BEATSTEP_COLORS_PINK;
    }
    if (color == "blue") {
      c = BEATSTEP_COLORS_BLUE;
    }
    bs->color(0x70 + led, c);
    std::cout << "OK" << std::endl;
  } else if (app.got_subcommand(subFw)) {
    bs->openPort(device - 1);
    std::vector<unsigned char> v = bs->version();
    std::cout << (int)v[0] << '.' << (int)v[1] << '.' << (int)v[2] << '.' << (int)v[3] << std::endl;
  } else if (app.got_subcommand(subGet)) {
    bs->openPort(device - 1);
    int r = bs->get(pp, cc);
    if (intOut) {
      std::cout << r << std::endl;
    } else {
      std::cout << std::hex << "0x" << r << std::endl;
    }
  } else if (app.got_subcommand(subSet)) {
    bs->openPort(device - 1);
    bs->set(pp, cc, vv);
    std::cout << "OK" << std::endl;
  } else if (app.got_subcommand(subLoad)) {
    bs->openPort(device - 1);
    n = bs->loadPreset(filename);
    std::cout << "OK" << std::endl;
  } else if (app.got_subcommand(subSave)) {
    bs->openPort(device - 1);
    n = bs->savePreset(filename);
    std::cout << "OK" << std::endl;
  }
  /*
  else if (app.got_subcommand(subUpdate)) {
    
    bs->openPort(device - 1);
    // TODO: get firmware version as a way to check it's in normal-mode before entering update-modde
    bs->updateMode();
    std::cout << "Your keyboard needs to be unplugged and plugged back in." << std::endl << "The lights will change." << std::endl << std::endl << "Press ENTER once you have done this." << std::endl;
    std::cin.get();
    bs->updateFirmware(filename);
    std::cout << "OK" << std::endl;
  }
  */

  else  if (app.got_subcommand(subCurses)) {

device = 2;
    
 initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, true);
  cbreak();
//  timeout(10);
  int aaa = 0;
  int bbb = 0;

  int x, y = 0;
  x = y = 0;
  int pad[16][7];
  std::string notes[12] = {"A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#"};

   move(y, x);


  
  int p = 6;
  int c = 0;
  bs->openPort(device - 1);

    
  //init pads
  for (int i = 0; i <= 15; i++)  {
  pad[i][4] = bs->get(i + 0x70, 3);
  pad[i][5] = bs->get(i + 0x70, 6);
  }

  int k = 0;
  while (k != 'x') {

//print pads 
  x = y = 0;
  for (int i = 0; i <= 15; i++)  {
  move(y, x);
  if (i == c) attron(A_REVERSE);
  if (pad[i][p-1] == 0) printw(" Toggle ");
  if (pad[i][p-1] == 1) printw("  Gate  ");
  if (i == c) attroff(A_REVERSE);
  move(y+1, x);
  printw("%d %s%d  ", pad[i][4], notes[pad[i][4]%12].data(), (pad[i][4]+9)/12);
  x=x+9; 
  if (i == 7) {
  y=+2;
  x=0;
  }
  }
  refresh();

  k = getch();

  switch(k) {
  case '.':
  bs->openPort(device - 1);
  delete bs;

  break;  

  case ',':
  if (pad[c][4] > 0) pad[c][4]--;
  bs->set(c+0x70, 3, pad[c][4]);
  break;

  case 't':
  pad[c][p-1] = 0;
  bs->set(c+0x70, p, pad[c][p-1]);
  break;

  case 'g':
  pad[c][p-1] = 1;
  bs->set(c+0x70, p, pad[c][p-1]);
  break;

  case 'T':
  pad[c][p-1] = 0;
  bs->set(c+0x70, p, pad[c][p-1]);
  c++;
  if (c==16) c = 0;
  break;

  case 'G':
  pad[c][p-1] = 1;
  bs->set(c+0x70, p, pad[c][p-1]);
  c++;
  if (c==16) c = 0;
  break;
  
  case KEY_RIGHT:
  c++;
  if (c==16) c = 0;
  break;

  case '\t':
  c++;
  if (c==16) c = 0;
  break;

  case KEY_LEFT:
  c--;
  if (c==-1) c = 15;
  break;

  case KEY_BTAB:
  c--;
  if (c==-1) c = 15;
  break;

  }
 
}
  endwin();
  
 }
 
  delete bs;
  return n ? 0 : 1;
}

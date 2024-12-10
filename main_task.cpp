#include <cstdio>
#include <cstring>
#include "main_task.h"

main_task::main_task()
        : task("Main", MAIN_STACKSIZE),
          _cs(SD_CS_PIN),
          _spi(SD_SPI0, SD_MISO_PIN, SD_MOSI_PIN, SD_SCLK_PIN, _cs),
          _sd(_spi),
          _next(NEXT_BUTTON),
          _part(PART_BUTTON),
          _ps(PICO_PS),
          _fs(_sd),
          _lcd_rst(LCD_RST_PIN),
          _lcd_dc(LCD_DC_PIN),
          _lcd_cs(LCD_CS_PIN),
          _lcd_spi(LCD_SPI1, LCD_MISO_PIN, LCD_MOSI_PIN, LCD_SCK_PIN, _lcd_cs),
          _lcd(_lcd_spi,_lcd_rst,_lcd_dc,ili9488_drv::PICO320),
          _gui(_lcd),
          _sel_index(0),
          _page_index(0),
          _last_page_index(0),
          _last_sel_index(0),
          update_sel(0),
          update_required(1),
          playing(0),
          last_play_pos(0),
          play_pos(0){

    _next.gpioMode(GPIO::INPUT | GPIO::PULLUP);
    _part.gpioMode(GPIO::INPUT | GPIO::PULLUP);
    _ps.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
    _gui.FontSelect(&FONT_8X14);
    _gui.SetForecolor(C_GREEN);
}

void main_task::draw_string(int x,int y,const char*str){
    _gui.FontSelect(&FONT_8X14);
    _gui.SetForecolor(C_GREEN);
    _gui.PutString(x,y,str);
}
void main_task::draw_char(int x,int y ,char c) {
    _gui.FontSelect(&FONT_8X14);
    _gui.PutChar(c,x,y,C_GREEN,C_BLACK, true);
}

void main_task::draw_bar(int x1, int x2,UG_COLOR c) {
   _gui.FillFrame(x1,280,x2,285,c);
}

int main_task::enum_files(char **fname_list, int *fsize_list) {
    char tmp[16];
    _gui.FontSelect(&FONT_8X14);
    _gui.ConsoleSetForecolor(C_GREEN);
    _gui.ConsoleSetBackcolor(C_BLACK);
    _gui.ConsolePutString("Init FatFS");
    FatFs::FRESULT res = _fs.mount(0);
    if(res == FatFs::FR_OK){
        _gui.ConsolePutString("[OK]");
    }else{
        sprintf(tmp,"[NG] code=0x%x",res);
        _gui.ConsolePutString(tmp);
        return -1;
    }
    _gui.ConsolePutString("\n");
    _gui.ConsolePutString("File List");

    int num_files = 0;
    res = _fs.findfirst(&_dir, &_finfo, "", "*.mp3");
    // Loop over all MP3 files
    while (res == FatFs::FR_OK && _finfo.fname[0] && strlen(_finfo.fname) > 4 ) {
        printf("%s\n",_finfo.fname);
        fname_list[num_files] = (char*)malloc(strlen(_finfo.fname) + 1);
        strcpy(fname_list[num_files], _finfo.fname);
        fsize_list[num_files] = _finfo.fsize;

        res = _fs.findnext(&_dir, &_finfo);
        num_files++;
    }
    if (res != FatFs::FR_OK) {
        sprintf(tmp, "[NG] code=0x%x", (int)res);
        _gui.ConsolePutString( tmp);
        return -1;
    }
    _fs.closedir(&_dir);
    _gui.ConsolePutString("[OK]");
    return num_files;
}

void truncate_string(const char *input, char *output, size_t max_length) {
    size_t input_len = strlen(input);

    if (input_len <= max_length) {
        strcpy(output, input);
    } else {
        strncpy(output, input, max_length - 3);
        output[max_length - 3] = '\0';
        strcat(output, "...");
    }
}

int main_task::select_mp3(int num_files, char **file_list) {
    char tmp[34];
    for (int i = 0; i < ITEMS_PER_PAGE && (_page_index + i) < num_files; i++) {
        truncate_string(file_list[_page_index+i],tmp,33);
        draw_string(20, i * 20, tmp);
    }

    return 0;
}
void main_task::draw_cursor() {
    char buf[12];
    sprintf(buf, "%02d", _sel_index + 1);
    draw_string(0,_lcd.getSizeY()-20,buf);

    if(update_sel){
        draw_string(0, (_last_sel_index % ITEMS_PER_PAGE) * 20, "  ");
    }
    draw_string(0, (_sel_index % ITEMS_PER_PAGE) * 20, "=>");
}

void main_task::boot_menu(int num_files, char **file_list) {
    _page_index = (_sel_index / ITEMS_PER_PAGE) * ITEMS_PER_PAGE;
    if(_page_index!= _last_page_index){
        _lcd.clearScreen(C_BLACK);
        _last_page_index = _page_index;
        update_required = 1;
    }

    if(update_required) {
        _lcd.clearScreen(C_BLACK);
        select_mp3(num_files,file_list);
        draw_cursor();
        update_required = 0;
        last_play_pos = 0;
        play_pos = 0;

    }

    if(update_sel){
        draw_cursor();
        update_sel = 0;
    }
}

void main_task::menu_up(int num_files) {
    _last_sel_index = _sel_index;
    if (_sel_index == 0) {
        _sel_index = num_files - 1;
    } else {
        _sel_index--;
    }
    update_sel = 1;
}
void main_task::menu_down(int num_files) {
    _last_sel_index = _sel_index;
    if (_sel_index == num_files - 1) {
        _sel_index = 0;
    } else {
        _sel_index++;
    }
    update_sel = 1;
}

void main_task::menu_start(char **fname_list ,sd_reader_task&sd_reader,mp3_decoder_task &decoder,pcm_pwm_rp2040_drv &pcm_drv) {
    FatFs::FRESULT res = FatFs::FR_OK;
    printf("start play: %s\n",fname_list[_sel_index]);
    res = _fs.open(&_file, fname_list[_sel_index], FA_OPEN_EXISTING | FA_READ);
    assert(res == FatFs::FR_OK);
    sd_reader.start(&_fs, &_file);
    decoder.reset();
    decoder.start();
    update_required = 1;
    playing =1;
    last_play_pos = 0;
    play_pos = 0;
}

void main_task::run() {
    char tmp[34];
    char *fname_list[MAX_FILES];
    int fsize_list[MAX_FILES];
    i2c_kbd _kbd;

    pcm_pwm_rp2040_drv pcm_drv(AUDIO_LEFT, AUDIO_RIGHT);
    sd_reader_task sd_reader;
    mp3_decoder_task decoder(pcm_drv, sd_reader);

    uint8_t keycheck = 0;
    uint8_t keyread = 0;
    uint8_t key_stat = 0;//press,release, or hold
    int kbd_ret = 0;
    int c;
    static int ctrlheld = 0;
    const char spinner[] = {'/', '-', '|', '\\'};
    uint8_t spin_i = 0;
    uint8_t spin_timer = 0;

    int play_pos_diff = 0;

    _lcd.clearScreen(C_BLACK);

    int num_files = enum_files( fname_list, fsize_list);
    if (num_files <= 0) {
        printf("num_files <=0\n");
        return;
    }
    while (true){
        if(playing){
            if (sd_reader.isAlive() &&  decoder.isAlive()) {
                //printf("playing now\n");

                if(update_required) {
                    _lcd.clearScreen(C_BLACK);
                    truncate_string(fname_list[_sel_index],tmp,33);
                    draw_string(20, 20, tmp);
                    draw_string(20, 40, "playing");
                    draw_string(20, 60, "press Esc to quit");
                    update_required = 0;
                }
                if(spin_timer >= 100) {
                    //(bitrate*8/)filesize
                    //draw bar use last_play_pos
                    //draw_bar(0,last_play_pos,C_BLACK);
                    play_pos = decoder.get_position(fsize_list[_sel_index],_lcd.getSizeY());
                    //draw bar use play_pos
                    play_pos_diff = play_pos - last_play_pos;
                    if(play_pos_diff < 0) {// VBR
                        draw_bar(play_pos,last_play_pos,C_BLACK);
                    }else{ //>= 0
                        //draw_bar(last_play_pos,play_pos,C_BLACK);
                        draw_bar(play_pos,play_pos+play_pos_diff,C_YELLOW);
                    }
                    draw_char(20, 300, spinner[spin_i % 4]);
                    spin_i++;
                    spin_timer = 0;
                    last_play_pos = play_pos;
                }
                spin_timer++;
            }else{
                printf("not playing...auto exit\n");
                _fs.close(&_file);
                playing =0;
                update_required =1;
            }
        }else{
            boot_menu(num_files,fname_list);
        }

        if(keycheck == 0) {
            if(keyread == 0) {
                kbd_ret = _kbd.write_i2c_kbd();
                keyread = 1;
            }else{
                kbd_ret = _kbd.read_i2c_kbd();
                keyread = 0;
            }
            keycheck = 16;
        }

        if(kbd_ret){
            if (kbd_ret == 0xA503)ctrlheld = 0;
            else if (kbd_ret == 0xA502) {
                ctrlheld = 1;
            } else if ((kbd_ret & 0xff) == 1) {//pressed
                key_stat = 1;
            } else if ((kbd_ret & 0xff) == 3) {
                key_stat = 3;
            }
            c = kbd_ret >> 8;
            int realc = -1;
            switch (c) {
                case 0xA1:
                case 0xA2:
                case 0xA3:
                case 0xA4:
                case 0xA5:
                    realc = -1;//skip shift alt ctrl keys
                    break;
                default:
                    realc = c;
                    break;
            }
            c = realc;
            if (c >= 'a' && c <= 'z' && ctrlheld)c = c - 'a' + 1;
            switch (c) {
                case 0xb5://UP
                    if(key_stat == 1) {
                        menu_up(num_files);
                    }
                    break;
                case 0xb6://DOWN
                    if(key_stat == 1) {
                        menu_down(num_files);
                    }
                    break;
                case 0xb4://LEFT
                    break;
                case 0xb7://RIGHT
                    break;
                case 0x0A://ENTER
                    if(key_stat == 1) {
                        menu_start(fname_list, sd_reader, decoder, pcm_drv);
                    }
                    break;
                case 0xB1://ESC
                    if(key_stat == 1) {
                        if (playing) {
                            playing = 0;
                            sd_reader.force_eof();
                            printf("stop playing\n");
                        }
                        update_required = 1;
                    }
                    break;
                default:
                    break;
            }
            kbd_ret = 0;
        }

        task::sleep_ms(1);
        if(keycheck) keycheck--;
    }

}

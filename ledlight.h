#ifndef LEDLIGHT_H
#define LEDLIGHT_H


class LedLight
{
public:
    LedLight();
    ~LedLight();

public:
    void horizotal();
    void vertical();
    void setLedTime(int time);

private:
    int fd;
};

#endif // LEDLIGHT_H

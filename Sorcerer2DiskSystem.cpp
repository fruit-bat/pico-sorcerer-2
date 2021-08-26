#include <pico/printf.h>

#include "Sorcerer2DiskSystem.h"

Sorcerer2DiskSystem::Sorcerer2DiskSystem() :
  _drives({
	  Sorcerer2DiskDrive(),
	  Sorcerer2DiskDrive(),
	  Sorcerer2DiskDrive(),
	  Sorcerer2DiskDrive()
  }),
  _activeDriveNumber(0x40),
  _activeDrive(0)
{
}

void Sorcerer2DiskSystem::tick() {
  for (int i = 0; i < 4; ++i ) {
	_drives[ i ].tick();
  }
  if (_activeDrive) {
	if (!_activeDrive->active()) {
		_activeDrive = 0;
		_activeDriveNumber = 0x40;
	}
  }
}

int Sorcerer2DiskSystem::readByte(int address) {
  printf("Disk system read %04X\n", address);

  return 0;	
}

void Sorcerer2DiskSystem::writeByte(int address, int value) {
  printf("Disk system write %04X %02X\n", address, value);

}

bool Sorcerer2DiskSystem::dataReady() {
  return _activeDrive ? _activeDrive->dataReady() : false;
}

bool Sorcerer2DiskSystem::home() {
  return _activeDrive ? _activeDrive->home() : false;
}

bool Sorcerer2DiskSystem::active() {
  return !!_activeDrive;
}

void Sorcerer2DiskSystem::stepForward() {
  if (_activeDrive) _activeDrive->stepForward();
}

void Sorcerer2DiskSystem::stepBackward() {
  if (_activeDrive) _activeDrive->stepBackward();
}

/*

    private readyWrite(): void {
        if ( this._activeDrive != null ) {
            this._activeDrive.readyWrite();
        }
    }

    private activate(drive: number): void {
        this._activeDriveNumber = drive;
        this._activeDrive = this._drives[ drive ];
        this._activeDrive.activate();
    }

    private writeReg0(b: number ): void {
        switch ( b ) {
            case 0xA0: break;
            case 0x20: this.activate( 0 );    break;
            case 0x21: this.activate( 1 );    break;
            case 0x22: this.activate( 2 );    break;
            case 0x23: this.activate( 3 );    break;
            case 0x60: this.stepBackward(); break;
            case 0x61: this.stepForward();    break;
            case 0x80: this.readyWrite();     break;
        }
    }

    private writeReg1(b: number): void {
        switch ( b ) {
            case 0xA0: break;
            case 0x20: this.activate( 0 );    break;
            case 0x21: this.activate( 1 );    break;
            case 0x22: this.activate( 2 );    break;
            case 0x23: this.activate( 3 );    break;
            case 0x60: this.stepBackward(); break;
            case 0x61: this.stepForward();    break;
            case 0x80: this.readyWrite();     break;
        }
    }

    private writeReg2(b: number): void {
        if ( this._activeDrive != null ) {
            this._activeDrive.writeReg2( b );
        }
    }

    private readReg0(): number {
        return this._activeDrive !== null ? this._activeDrive.readReg0() : 0;
    }

    private readReg1(): number {
        let r = this._activeDriveNumber;

        if ( this.active() ) r |= 0x20;
        if ( this.home() ) r |= 0x08;
        if ( this.dataReady() ) r |= 0x80;

        return r;
    }

    private readReg2(): number {
        return this._activeDrive === null ? 0 : this._activeDrive.readReg2();
    }

    public writeByte(address: number, b: number): void {
        switch (address - MEM_DISK_REG_START) {
            case 0: this.writeReg0( b ); break;
            case 1: this.writeReg1( b ); break;
            case 2: this.writeReg2( b ); break;
        }
    }

    public readByte(address: number): number {
        switch (address - MEM_DISK_REG_START) {
            case 0: return this.readReg0();
            case 1: return this.readReg1();
            case 2: return this.readReg2();
        }
        return 0;
    }


    */

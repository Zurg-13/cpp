import { Injectable } from '@angular/core';
import { HttpService } from './http.service';

/*
  Сервис для хранения конфигурации и упрвления ею
*/

export interface AppConf {
  wshost: string;
  wsport: string;
}

export interface AppInfo {
  sysdate: string;
  appname: string;
  version: string;
}

@Injectable({
  providedIn: 'root'
})
export class ConfigService {

  private infoUrl: string; // Адрес для получения системной информации
  private confUrl: string; // Адрес для получения конфига от сервера
  private wsUrl: string; // Адрес для соединения по вебсокету

  public appConf: AppConf;
  public appInfo: AppInfo;

  constructor(
    private httpService: HttpService
  ) { }

  public getWsUrl(): string {
    return this.wsUrl;
  }

  public getInfoUrl(): string {
    return this.infoUrl;
  }

  public getConfUrl(): string {
    return this.confUrl;
  }

  // Формирование адреса ws
  private setWsUrl(wshost: string, wsport: string) {
    this.wsUrl = 'ws://' + wshost + ':' + wsport;
  }

  // Получение конфигурации от сервера
  public getAppConf(): Promise<void> {
    return new Promise<void>((resolve: Function, reject: Function) => {
      this.httpService.get(this.confUrl).toPromise()
      .then((response: AppConf) => {
          console.log(response);
          this.appConf = {
            wshost: response.wshost,
            wsport: response.wsport
          }
          // Формируем адрес для ws
          this.setWsUrl(response.wshost, response.wsport);
          resolve();
      }).catch((response: any) => {
          console.error(response);
          reject(`Ошибка: ${JSON.stringify(response)}`);
      });
    });
  }

  // Получение системной информации от сервера
  public getAppInfo(): Promise<void> {
    return new Promise<void>((resolve: Function, reject: Function) => {
      this.httpService.get(this.infoUrl).toPromise()
      .then((response: AppInfo) => {
          console.log(response);
          this.appInfo = {
            appname: response.appname
          , version: response.version
          , sysdate: response.sysdate
          };
          resolve();
      }).catch((response: any) => {
          console.error(response);
          reject(`Ошибка: ${JSON.stringify(response)}`);
      });
    });
  }


  // Загрузка первоначальной конфигурации нашего приложения (это локальный json файл)
  public load(): Promise<void> {
    console.log('load');
    const jsonFile = 'assets/config.json';
    return new Promise<void>((resolve, reject) => {
      this.httpService.get(jsonFile).toPromise()
      .then((response: any) => {
          console.log(response);
          this.infoUrl = response.infoUrl;
          this.confUrl = response.confUrl;
          resolve();
      }).catch((response: any) => {
          console.error(response);
          reject(`Не удалось загрузить конфигурацию из файла '${jsonFile}': ${JSON.stringify(response)}`);
      });
    });
  }
}

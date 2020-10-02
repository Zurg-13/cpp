import { Injectable } from '@angular/core';
import { HttpService } from './http.service';
import { WsctService } from './wsct.service';


export interface AppConf { host: string; port: string; }
export interface AppInfo { date: string; name: string; vrsn: string; }

@Injectable({ providedIn: 'root' })

/* Сервис управления конфигурацией. *******************************************/
/******************************************************************************/
export class ConfService {

  private infoUrl: string; // Адрес для получения системной информации
  private confUrl: string; // Адрес для получения конфига от сервера
  private wsсtUrl: string; // Адрес для соединения по вебсокету

  public conf: AppConf;
  public info: AppInfo;

  // Конструктор. --------------------------------------------------------------
  //----------------------------------------------------------------------------
  constructor(
    private http_svc: HttpService
  , private wsct_svc: WsctService
  ) { 

      // Послать запрос на сервер и получить конфигурацию
      this.load
      ().then(() => {
        return this.getAppConf();

      }).then(() => {
  
        // Настройка WS.
        console.log(this.conf);
        this.wsct_svc.closeConnection();
        this.wsct_svc.create(this.getWsctUrl());
        this.wsct_svc.init();

      }).catch(() => {
        console.log('Ошибка получения конфигурации');
      });
  }// constructor

  public getWsctUrl(): string { return this.wsсtUrl; }
  public getInfoUrl(): string { return this.infoUrl; }
  public getConfUrl(): string { return this.confUrl; }

  // Формирование адреса WS. --------------------------------------------------- 
  //----------------------------------------------------------------------------
  private setWsUrl(host: string, port: string) 
    { this.wsсtUrl = 'ws://' + host + ':' + port; }

  // Получение конфигурации от сервера. ----------------------------------------
  //----------------------------------------------------------------------------
  public getAppConf(): Promise<void> {
    return new Promise<void>((resolve: Function, reject: Function) => {
      this.http_svc.get(this.confUrl).toPromise
      ().then((response: AppConf) => {
          this.conf = {
            host: response.host
          , port: response.port }

          // Формируем адрес для ws
          this.setWsUrl(response.host, response.port);
          resolve();

      }).catch((response: any) => {
          console.error(response);
          reject(`Ошибка: ${JSON.stringify(response)}`);
      });
    });
  }// getAppConf

  // Получение системной информации от сервера. --------------------------------
  //----------------------------------------------------------------------------
  public getAppInfo(): Promise<void> {
    return new Promise<void>((resolve: Function, reject: Function) => {
      this.http_svc.get(this.infoUrl).toPromise
      ().then((response: AppInfo) => {
          this.info = {
            name: response.name
          , vrsn: response.vrsn
          , date: response.date };
          resolve();
      }).catch((response: any) => {
          reject(`Ошибка: ${JSON.stringify(response)}`);
      });
    });
  }// getAppInfo

  // Загрузка первоначальной конфигурации приложения (локальный json). ---------
  //----------------------------------------------------------------------------
  public load(): Promise<void> {
    const jsonFile = 'assets/config.json';
    
    return new Promise<void>((resolve, reject) => {
      this.http_svc.get(jsonFile).toPromise
      ().then((response: any) => {
          this.infoUrl = response.infoUrl;
          this.confUrl = response.confUrl;
          resolve();

      }).catch((response: any) => {
          reject(`Не удалось загрузить конфигурацию ${jsonFile}: ${JSON.stringify(response)}`);
      });
    });
  }// load

}// ConfService

import { Injectable } from '@angular/core';
import { webSocket, WebSocketSubject } from "rxjs/webSocket";
import { Subject, throwError, Observable } from 'rxjs';
import { catchError, retryWhen, repeatWhen, delay, take, concat  } from 'rxjs/operators';
import { Router } from '@angular/router';
import { Cmnd } from '../cls/cmnd';

/*
  Сервис для взаимодействия с сервером по протоколу ws (вебсокет)
*/

@Injectable({
  providedIn: 'root'
})
export class WebsocketService {

  public instance: WebSocketSubject<any>;
  public connectionStatus: boolean = false;
  public wsUrl: string;
  public openObserver: Subject<Event> = new Subject<Event>();
  public closeObserver: Subject<Event> = new Subject<Event>();
  public onMessageObserver: Subject<Event> = new Subject<Event>(); 
  public router: Router;
  private intervalInstance: any;

  // Создание instance webSocket по указанному URL
  public create(url: string): void {
    this.connectionStatus = false;
    console.log('create websocket...');
    this.wsUrl = url;
    try {
      this.instance = webSocket({
        url: this.wsUrl, 
        deserializer: msg => msg,
        openObserver: this.openObserver,
        closeObserver: this.closeObserver
      });
    }
    catch(ex) {
      console.log('Ошибка при открытии websocket');
      console.log(ex);
    }
    console.log(this.instance);
  }

  // Закрытие соединения
  public closeConnection(): void {
    if(this.instance) {
      console.log('Закрытие соединения методом complete');
      this.instance.complete();
      this.instance = null;
    }
  }

  // Отправка запроса на вебсокет
  public sendMessage(data: Cmnd) {
    if(this.instance) {
      try {
        this.instance.next(data);
        console.log('Сообщение отправлено request = ' + data);
      }
      catch(ex) {
        console.error(ex);
      }
    }
  }

  // Добавление внешнего обработчика на событие onMessage
  /*
  public onMessage(action: Function) {
    this.onMessageObserver.subscribe((data: any) => {
        action(data);
    });
  }
  */

  // Инициализация основной логики управления соденинением вебсокета
  public init(): void {
    this.instance
    .pipe( 

      // Реакция на ошибку присланную по вебсокету
      retryWhen(error => error.pipe(delay(8000), concat(throwError(error))))
    )
    .pipe(

      // Реакция на закрытие соединения и попытка его восстановить
      repeatWhen(complete => complete.pipe(delay(8000), concat(throwError(complete))))
    )
    .subscribe (

      // Обработчик по умолчанию, просто для того чтобы было видно что данные пришли,
      // если других обработчиков никто не навесил
      (message: MessageEvent) => {
        // Неотловленная ошибка в этом блоке (subscribe) вызывает обрыв вебсокета, которое автоматически не восстанавливается!
        try {
          console.log('message received: ' + message.data);
          this.onMessageObserver.next(JSON.parse(message.data)); 
        } catch(ex) {
          console.error(ex);
        }
      },
      (errors: ErrorEvent) => {
        console.log(errors);
      },
      () => {
        // Реакция на закрытие соединения. 
        // Просто выводим информацию по этому событию
        // Восстановлением соединения займётся другой код
        console.log('complete.');
      }
    );
  }

  constructor() { 
    // Подписываемся на событие успешного открытия вебсокета
    this.openObserver.subscribe(() => {      
      console.log('openObserver...');
      this.connectionStatus = true;
    });

    // Событие закрытия соединения по вебсокету
    this.closeObserver.subscribe((error) => {
      console.log('close websocket.');
      console.log(error);      
      this.connectionStatus = false;
      clearInterval(this.intervalInstance);
    });
  }
}

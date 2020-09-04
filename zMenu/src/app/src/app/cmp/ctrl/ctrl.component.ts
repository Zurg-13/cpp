import { Component, OnInit } from '@angular/core';
import { Item } from 'src/app/cls/Item';
import { ConfigService } from 'src/app/srv/config.service';
import { WebsocketService } from 'src/app/srv/websocket.service';


@Component({
  selector: 'app-ctrl',
  templateUrl: './ctrl.component.html',
  styleUrls: ['./ctrl.component.scss']
})
export class CtrlComponent implements OnInit {

  public items: Item[] = []; // Список редактируемых карточек item

  constructor(
    private configService: ConfigService,
    private websocketService: WebsocketService
  ) { }

  ngOnInit(): void {
    // Ручное добавление элементов item, для отладки интерфейса (можно удалить)
    // for(let i = 0; i < 5; i++) {
    //   let newItem: Item = {
    //     id: i,
    //     name: 'Имя ' + i,
    //     note: 'Описание' + i,
    //     cost: 99,
    //     room: null,
    //     type: null
    //   };
    //   this.items.push(newItem);
    // }

    // Послать запрос на сервер и получить конфигурацию
    this.configService.load()
    .then(() => {
      console.log('Конфигурация обработана');
      // Получаем конфигурацию от сервера
      return this.configService.getAppConf();
    })
    .then(() => {
      // Настраиваем вебсокет
      console.log(this.configService.appConf);
      this.websocketService.closeConnection();
      this.websocketService.create(this.configService.getWsUrl());
      this.websocketService.init();
      // Отправляем запрос на получение списка item
      this.websocketService.sendMessage({method: 'item'});

      // Подписка нашего обработчика сообщений
      this.subscribeOnMessage();
    })
    .catch(() => {
      console.log('Ошибка получения конфигурации');
    });
  }

  private subscribeOnMessage(): void {
    if(this.websocketService.instance != null) {
      this.websocketService.onMessage((data: any) => {
        console.log('subscribe OnMessage');
        console.log(data);
        for(let i = 0; i < data.length; i++) {
          let currentItem = data[i];
          let newItem: Item = {
            id: currentItem.id,
            name: currentItem.name,
            note: currentItem.note,
            cost: currentItem.cost,
            room: currentItem.room,
            type: currentItem.type
          };
          this.items.push(newItem);
        }
      });
    }
    else {
      console.log('instance вебсокета не определён.');
    }
  }


  // Добавление item в список
  public createItem(): void {
    /*
      Идентификатор нужно получить из базы.
      То-есть, сперва выполнить запрос к серверу для создания,
      а он уже вернёт id.
    */
    let newIndex = this.items.length;

    let newItem: Item = {
      id: newIndex,
      name: 'Имя ' + newIndex,
      note: 'Описание' + newIndex,
      cost: 99,
      room: null,
      type: null
    };
    
    this.items.push(newItem);
  }

}

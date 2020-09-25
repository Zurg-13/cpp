import { Component, OnInit } from '@angular/core';
import { Item } from 'src/app/cls/Item';
import { ConfigService } from 'src/app/srv/config.service';
import { WebsocketService } from 'src/app/srv/websocket.service';
import { ModelService } from 'src/app/srv/model.service';


@Component({
  selector: 'app-ctrl',
  templateUrl: './ctrl.component.html',
  styleUrls: ['./ctrl.component.scss']
})
export class CtrlComponent implements OnInit {

/*  
  public items: Item[] = []; // Список редактируемых карточек item
*/  

  constructor(
    private configService: ConfigService
  , private websocketService: WebsocketService
  , public modelService: ModelService
  ) {}

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
      this.websocketService.sendMessage({cmnd: 'item_list', data: null});

      // Подписка нашего обработчика сообщений
      this.subscribeOnMessage();
    })
    .catch(() => {
      console.log('Ошибка получения конфигурации');
    });
  }

  private subscribeOnMessage(): void {
    if (this.websocketService.instance != null) {

      this.websocketService.onMessageObserver.subscribe((data: any) => {
        if(data.cmnd != "item_list") { return; }

        console.log(data);

        for(let i = 0; i < data.list.length; i++) {
          let currentItem = data.list[i];
          let newItem: Item = {
            id: currentItem.id
          , name: currentItem.name, note: currentItem.note
          , cost: currentItem.cost
          , stat: currentItem.stat, hide: currentItem.hide
          , room: currentItem.room, rnme: currentItem.rnme
          , type: currentItem.type, tnme: currentItem.tnme
          };

          this.modelService.setItem(newItem);
/*          
          this.items.push(newItem);
*/          
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
/*   
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
*/    
  }

}

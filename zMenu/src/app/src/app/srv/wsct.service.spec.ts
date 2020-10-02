import { TestBed } from '@angular/core/testing';
import { WsctService } from './wsct.service';

describe('WebsocketService', () => {
  let service: WsctService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(WsctService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});

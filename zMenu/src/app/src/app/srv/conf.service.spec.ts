import { TestBed } from '@angular/core/testing';

import { ConfService } from './conf.service';

describe('ConfService', () => {
  let service: ConfService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ConfService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});

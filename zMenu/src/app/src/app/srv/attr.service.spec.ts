import { TestBed } from '@angular/core/testing';

import { AttrService } from './attr.service';

describe('AttrService', () => {
  let service: AttrService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(AttrService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});

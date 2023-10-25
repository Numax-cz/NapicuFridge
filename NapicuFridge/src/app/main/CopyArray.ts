/**
 * @author tim-montague
 */
export function CopyArray(aObject: any): any {
  if (!aObject) {
    return aObject;
  }

  let v;
  let bObject: any = Array.isArray(aObject) ? [] : {};
  for (const k in aObject) {
    v = aObject[k];
    bObject[k] = typeof v === 'object' ? CopyArray(v) : v;
  }

  return bObject;
}

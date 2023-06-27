import {AnimationController} from "@ionic/angular";

const animationCtrl = new AnimationController();


export const app_animation = ((_: HTMLElement, opts: any) => {
  // create root transition
  const rootTransition = animationCtrl
    .create()
    .duration(opts.duration || 200)
    .easing('cubic-bezier(0.7,0,0.3,1)');

  const enterTransition = animationCtrl.create().addElement(opts.enteringEl);
  const exitTransition = animationCtrl.create().addElement(opts.leavingEl);

  enterTransition.fromTo('opacity', '0', '1');
  exitTransition.fromTo('opacity', '1', '0');

  let value: number = 2.5;

  if (opts.direction === 'forward') {
    enterTransition.fromTo('transform', `translateY(${value}%)`, 'translateY(0%)');
    exitTransition.fromTo('transform', 'translateX(0%)', `translateY(-${value}%)`);
  } else {
    enterTransition.fromTo('transform', `translateY(-${value}%)`, 'translateY(0%)');
    exitTransition.fromTo('transform', 'translateY(0%)', `translateY(${value}%)`);
  }

  rootTransition.addAnimation([enterTransition, exitTransition]);
  return rootTransition;
});
